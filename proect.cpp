// proect.cpp: ���������� ����� ����� ��� ����������� ����������.
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
	cvMatchTemplate( frame, templ, result, CV_TM_CCOEFF_NORMED);//��� ����, ��� ����, ���� ���������, ����� ��������.
	//�������� - ���������� ����������� � ������� �� ������������, � �������������.
	//cvNormalize( result, result, 1, 0, CV_MINMAX ); 
	//����������, ���������� - ������� �� 1. ������� ��� �������, � ��� ���� ������, ����� �� �� ����������� ���������.
	//cvShowImage( "Image", result );  //����� ����� ��������, ��� �� ����
	double min,max;
	CvPoint minpos, maxpos;
	cvMinMaxLoc(result,&min,&max,&minpos,&maxpos);
	//CvPoint dot1 = cvPoint(maxpos.x-41,maxpos.y-20); ����� ��� �������� ����� �������. ��������� - ����� ������ ����.
	CvPoint dot2=cvPoint(maxpos.x+83,maxpos.y+61);
	printf("%5.3f; x = %d; y = %d \n",max,maxpos.x,maxpos.y); //����� ���������. ���� �� ����
	if (max>0.5){//�������� ��������������� �������������, �� ���� ��� �����.
		cvRectangle(frame, /*dot1*/maxpos,dot2,CV_RGB(0,255,0),2);
	}
	//cvWaitKey(0);//���� 0 �� ���� ��������. �� ����.
}

int _tmain(int argc, _TCHAR* argv[])
{
	 // ��� ����� ������� ������ ����������
        wchar_t* filenam = argc == 2 ? argv[1] : L"test.avi";//�������� � ���������
		int sizew=wcstombs(NULL,filenam, 0);//�� wchar � char
		char *filename = new char[sizew+1];
		wcstombs(filename, filenam, sizew+1);

        printf("[i] file: %s\n", filename);

		templ = cvLoadImage("look-2.png",1);

        // ���� ��� ����������� ��������
        cvNamedWindow("original",CV_WINDOW_AUTOSIZE);//��� ����, ��� ������ - �� �����.

        // �������� ���������� � �����-�����
        CvCapture* capture = cvCreateFileCapture( filename );//���������, � ������� �� ����� ����� ��������
		//��� ���� � �����

        while(1){
                // �������� ��������� ����
                frame = cvQueryFrame( capture ); 
                if( !frame ) {
                        break;
                }
                // ����� ����� ��������
                // ��������� ���������
				//��� � � ������)
				find_a_part();
                // ���������� ����
                cvShowImage( "original", frame );//��� ����, ������ ������� �������.

                char c = cvWaitKey(4);//�������� ����� ���� ������. ����� 25 ������/�(1000/25=40).
				//���������, ������� � ��� �������� �����)
				//���� �� ������� ��� ����.
                if (c == 27) { // ���� ������ ESC - �������
                        break;
                }
        }
        // ����������� �������
        cvReleaseCapture( &capture );//������� ���������� ��������� �����
        // ������� ����
        cvDestroyWindow("original");
	return 0;
}