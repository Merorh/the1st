// proect.cpp: определ€ет точку входа дл€ консольного приложени€.
//

#include "stdafx.h"
#include <cv.h>
#include <highgui.h>
#include <stdlib.h>
#include <stdio.h>
IplImage* frame =0, *templ=0,*result=0;

void find_a_part()
{
	result = cvCreateImage(cvSize( frame->width - templ->width + 1, frame->height - templ->height + 1),32,1);
	cvMatchTemplate( frame, templ, result, CV_TM_CCOEFF_NORMED);//где ижем, что ищем, куда результат, какой алгоритм.
	//алгоритм - коррел€ци€ результатов в расчете на максимальный, с нормированием.
	//cvNormalize( result, result, 1, 0, CV_MINMAX ); 
	//нормировка, наибольшее - беретс€ за 1. полезно при статике, а мне надо убрать, чтобы не те изображени€ отсеивать.
	//cvShowImage( "Image", result );  //вывод карты схожести, нам не надо
	double min,max;
	CvPoint minpos, maxpos;
	cvMinMaxLoc(result,&min,&max,&minpos,&maxpos);
	//CvPoint dot1 = cvPoint(maxpos.x-41,maxpos.y-20); думал что отмечает центр области. оказалось - левый нижний угол.
	CvPoint dot2=cvPoint(maxpos.x+83,maxpos.y+61);
	printf("%5.3f; x = %d; y = %d \n",max,maxpos.x,maxpos.y); //вывод координат. тоже не надо
	if (max>0.5){//выставил чуствительность индивидуально, по тому что видел.
		cvRectangle(frame, /*dot1*/maxpos,dot2,CV_RGB(0,255,0),2);
	}
	//cvWaitKey(0);//если 0 то ждем вечность. не надо.
}

int _tmain(int argc, _TCHAR* argv[])
{
	 // им€ файла задаЄтс€ первым параметром
        wchar_t* filenam = argc == 2 ? argv[1] : L"test.avi";//записали в аргументы
		int sizew=wcstombs(NULL,filenam, 0);//из wchar в char
		char *filename = new char[sizew+1];
		wcstombs(filename, filenam, sizew+1);

        printf("[i] file: %s\n", filename);

		templ = cvLoadImage("look-2.png",1);

        // окно дл€ отображени€ картинки
        cvNamedWindow("original",CV_WINDOW_AUTOSIZE);//им€ окна, его размер - по видео.

        // получаем информацию о видео-файле
        CvCapture* capture = cvCreateFileCapture( filename );//структура, в которую по имени файла груз€тс€
		//вс€ инфа о файле

        while(1){
                // получаем следующий кадр
                frame = cvQueryFrame( capture ); 
                if( !frame ) {
                        break;
                }
                // здесь можно вставить
                // процедуру обработки
				//что € и сделаю)
				find_a_part();
                // показываем кадр
                cvShowImage( "original", frame );//им€ окна, обьект который вывести.

                char c = cvWaitKey(4);//задержка перед след кадром. стоит 25 кадров/с(1000/25=40).
				//уменьшаем, функци€ и так тормозит кадры)
				//либо мы жмакаем что ниже.
                if (c == 27) { // если нажата ESC - выходим
                        break;
                }
        }
        // освобождаем ресурсы
        cvReleaseCapture( &capture );//местный деструктор структуры видео
        // удал€ем окно
        cvDestroyWindow("original");
	return 0;
}