// WidzenieMaszynowe.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include "pch.h"
#include <iostream>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int main()
{
	Mat zdj1,zdj2,zdj3,zdj4,pom1,pom2,pom3,pom4,pom5,pom6,pom7,pom8,pom9,pom10,crop,crop2,wzor1,wzor2,wzor3,wzor4;
	Mat numer1, numer2, numer3, numer4;
	Mat a = getStructuringElement(MORPH_ELLIPSE, Size(20, 20));
	vector<vector<Point> > V,V1,V2,V3,V4,V5,V6,V7,V8;
	int swamp = 0;
	int mountain = 0;
	int island = 0;
	int forest = 0;
	bool n1 = false;

	zdj1=imread("mtg14.jpg",IMREAD_GRAYSCALE);					// wczytanie karty
	pom2 = zdj1.clone();
    threshold(zdj1, zdj1, 150, 255, THRESH_BINARY);

	pom1 = zdj1.clone();
	pom5 = zdj1.clone();
	zdj2 = zdj1.clone();
	pom8 = zdj2.clone();
	
	threshold(pom1, pom1, 255, 255, THRESH_BINARY);
	pom3 = pom1.clone();

	namedWindow("zdj4", WINDOW_NORMAL);
	imshow("zdj4", zdj1);
	waitKey(0);

	medianBlur(zdj1, zdj1, 5);								// obróbka wczytanego zdjecia w celu możliwości znalezienia linii
	namedWindow("zdj", WINDOW_NORMAL);
	imshow("zdj", zdj1);
	waitKey(0);

	Canny(zdj1, zdj1, 50, 200, 3);
	namedWindow("zdj100", WINDOW_NORMAL);
	imshow("zdj100", zdj1);
	waitKey(0);

	vector<Vec4i> lines;
	int dlugosc,poziomosc,wysokosc1,wysokosc2;
	int mini = 100000;
	int maxi = 0;
	vector<int> T;
	bool czy_obrocony;
	
	HoughLinesP(zdj1,lines, 1, CV_PI / 180, 50, 50, 10);					// transformata Hougha żeby znależć linie w celu wyznaczenia obszaru gdzie jest mana
	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		dlugosc = norm(Point(l[0], l[1]) - Point(l[2], l[3]));
		poziomosc = (Point(l[0], l[1]).y - Point(l[2], l[3]).y);
		if (dlugosc > 120 && abs(poziomosc) < 40) {														// branie pod uwagę tylko wystarczająco długich i poziomych linii
			line(pom1, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255, 255, 255), 1, CV_AA);
			if (Point(l[0], l[1]).y < mini)															// znalezienie linii najwyzej i najnizej i ich wspolrzednej y
				mini = Point(l[0], l[1]).y;
			if (Point(l[0], l[1]).y >maxi)
				maxi = Point(l[0], l[1]).y;
			T.push_back(Point(l[0], l[1]).y);
		}
	}

	if (mini > zdj1.size().height-maxi) {															// określenie czy karta jest obrócona czy nie i obrócenie jeśli była
		czy_obrocony = true;
		cout << "Obrocona karta" << endl;
		flip(zdj1, zdj1, -1);
		flip(pom2, pom2, -1);
		T.clear();
		HoughLinesP(zdj1, lines, 1, CV_PI / 180, 50, 50, 10);
		for (size_t i = 0; i < lines.size(); i++)
		{
			Vec4i l = lines[i];
			dlugosc = norm(Point(l[0], l[1]) - Point(l[2], l[3]));
			poziomosc = (Point(l[0], l[1]).y - Point(l[2], l[3]).y);
			if (dlugosc > 130 && poziomosc < 40) {
				line(pom3, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255, 255, 255), 1, CV_AA);
				T.push_back(Point(l[0], l[1]).y);
			}
		}
		pom1 = pom3;
	}
	else {
		czy_obrocony = false;
		cout << "Normalna karta" << endl;
	}

	sort(T.begin(), T.end());					// znalezienie dwóch pierwszych od góry linii

	wysokosc1 = T[0];						
	wysokosc2 = T[1];

	namedWindow("zdj7", WINDOW_NORMAL);
	imshow("zdj7", pom1);
	waitKey(0);

	Rect croppedRectangle = Rect(165,wysokosc2-((wysokosc2-wysokosc1)/2)-5,80,wysokosc2-wysokosc1-7);				// wycięcie obszaru many
	crop = pom2(croppedRectangle);
	bitwise_not(crop, crop);
	crop2 = pom8(croppedRectangle);
	bitwise_not(crop2, crop2);

	adaptiveThreshold(crop, crop, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 101, -61);
	pom6 = crop.clone();
	threshold(pom6, pom6, 255, 255, THRESH_BINARY);

	namedWindow("zdj77", WINDOW_NORMAL);
	imshow("zdj77", crop);
	waitKey(0);

	wzor1 = imread("mountain.jpg", IMREAD_GRAYSCALE);							// wczytanie wzorcow symboli i przeskalowanie ich do rozmiaru wycietego obszaru
	threshold(wzor1, wzor1, 150, 255, THRESH_BINARY);
	bitwise_not(wzor1, wzor1);
	wzor2 = imread("swamp.jpg", IMREAD_GRAYSCALE);
	threshold(wzor2, wzor2, 150, 255, THRESH_BINARY);
	bitwise_not(wzor2, wzor2);
	wzor3 = imread("forest.png", IMREAD_GRAYSCALE);
	threshold(wzor3, wzor3, 150, 255, THRESH_BINARY);
	bitwise_not(wzor3, wzor3);
	wzor4 = imread("island.png", IMREAD_GRAYSCALE);
	threshold(wzor4, wzor4, 150, 255, THRESH_BINARY);
	bitwise_not(wzor4, wzor4);
	numer1 = imread("1.png", IMREAD_GRAYSCALE);
	threshold(numer1, numer1, 150, 255, THRESH_BINARY);
	bitwise_not(numer1, numer1);

	morphologyEx(numer1, numer1, MORPH_DILATE, a);
	resize(numer1, numer1, Size(crop.size().height, crop.size().height));

	resize(wzor1, wzor1, Size(crop.size().height, crop.size().height));
	resize(wzor2, wzor2, Size(crop.size().height, crop.size().height));
	resize(wzor3, wzor3, Size(crop.size().height, crop.size().height));
	resize(wzor4, wzor4, Size(crop.size().height, crop.size().height));

	pom7 = wzor1.clone();
	threshold(pom7, pom7, 255, 255, THRESH_BINARY);

	findContours(crop, V, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	findContours(wzor1, V1, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	findContours(wzor2, V2, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	findContours(wzor3, V3, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	findContours(wzor4, V4, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

	findContours(numer1, V5, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);


	drawContours(pom6, V, -1, (255, 255, 255));

	namedWindow("zdj4441", WINDOW_NORMAL);
	imshow("zdj4441", pom6);
	waitKey(0);


	drawContours(pom7, V2, -1, (255, 255, 255));
	
	namedWindow("zdj444", WINDOW_NORMAL);
	imshow("zdj444", pom7);
	waitKey(0);

	for(int i = 0; i < V.size(); i++) {													//sprawdzanie symboli czy występują dobranymi kryteriami
		for (int j = 0; j < V1.size(); j++) {
			if (contourArea(V[i]) >20  && contourArea(V1[j]) > 20) {
				if (matchShapes(V[i], V1[j], 1, 0.0) < 0.4)
					mountain++;
			}

		}
	}
	for (int i = 0; i < V.size(); i++) {
		for (int j = 0; j < V2.size(); j++) {
			if (contourArea(V[i]) > 10 && contourArea(V2[j]) > 10) {
				if (matchShapes(V[i], V2[j], 1, 0.0) < 0.09)
					swamp++;
			}
		}
	}
	for (int i = 0; i < V.size(); i++) {
		for (int j = 0; j < V3.size(); j++) {
			if (contourArea(V[i]) > 7 && contourArea(V3[j]) > 7) {
				if (matchShapes(V[i], V3[j], 1, 0.0) < 0.13)
					forest++;
			}

		}
	}
	for (int i = 0; i < V.size(); i++) {
		for (int j = 0; j < V4.size(); j++) {
			if (contourArea(V[i]) > 7 && contourArea(V4[j]) > 7) {
				if (matchShapes(V[i], V4[j], 1, 0.0) < 0.1)
				island++;
			}
		}
	}

	for (int i = 0; i < V.size(); i++) {
		for (int j = 0; j < V5.size(); j++) {
			if (arcLength(V[i], 1) > 10 && arcLength(V5[j], 1) > 10) {
				if (matchShapes(V[i], V5[j], 1, 0.0) < 4)
					n1 = true;
			}
		}
	}


	cout << endl << endl;															// wyświetlenie jaka mana wystąpiła
	if (n1 == true) cout << "Jakiekolwiek:1" << endl;
	if(swamp!=0) cout << "Swampy:" << swamp << endl;
	if (forest != 0) cout << "Foresty:" << forest << endl;
	if (island != 0) cout << "Islandy:" << island << endl;
	if (mountain != 0) cout << "Mountainy:" << mountain << endl;

	return 0;
}

	/*
					cout << matchShapes(V[i], V1[j], 1, 0.0) << endl;
	
	img1 = cv2.imread('star.jpg', 0)
	 img2 = cv2.imread('star2.jpg', 0)
 ret, thresh = cv2.threshold(img1, 127, 255, 0)
		 ret, thresh2 = cv2.threshold(img2, 127, 255, 0)
		 contours, hierarchy = cv2.findContours(thresh, 2, 1)
		 cnt1 = contours[0]
		 contours, hierarchy = cv2.findContours(thresh2, 2, 1)
	 cnt2 = contours[0]
		 ret = cv2.matchShapes(cnt1, cnt2, 1, 0.0) */





	/*morphologyEx(zdj1, zdj1, MORPH_OPEN, a);
	namedWindow("zdj12", WINDOW_NORMAL);
	imshow("zdj12", zdj1);
	waitKey(0); 

	

	pom1 = zdj1.clone();
	threshold(pom1, pom1, 255, 255, THRESH_BINARY);
	findContours(zdj1, V, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	int szukane ;
	int max = 0;
	for (int i = 0; i < V.size(); i++) {
		if (arcLength(V[i], 1) > max) {
			max = arcLength(V[i], 1);
			szukane = i;
		}
	}
	drawContours(pom1, V, -1, (255, 255, 255));
	/*
			cout << V[szukane].size() << endl;
			cout << V[szukane][3] << endl;
			V1.push_back(V[szukane]);
	for (int i = 0; i < V[szukane].size(); i++) {
				drawContours(pom1, V,szukane, (0, 0, 255));
			} 
	namedWindow("image2", WINDOW_NORMAL);
	imshow("image2", pom1);
	waitKey(0);

	namedWindow("image123", WINDOW_NORMAL);
	imshow("image123", pom1); 
	waitKey(0); */

/*
	Threshold(roznica,roznica,10,255,THRESH_BINARY);

	  Mat x = imread("../data/wm_lab_2/1.jpg",IMREAD_GRAYSCALE);

	  adaptiveThreshold(x,y,255,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY,101,-35)
	
	drawContours(klat3,V,-1,(0,0,255))

	*/


// Uruchomienie programu: Ctrl + F5 lub menu Debugowanie > Uruchom bez debugowania
// Debugowanie programu: F5 lub menu Debugowanie > Rozpocznij debugowanie

// Porady dotyczące rozpoczynania pracy:
//   1. Użyj okna Eksploratora rozwiązań, aby dodać pliki i zarządzać nimi
//   2. Użyj okna programu Team Explorer, aby nawiązać połączenie z kontrolą źródła
//   3. Użyj okna Dane wyjściowe, aby sprawdzić dane wyjściowe kompilacji i inne komunikaty
//   4. Użyj okna Lista błędów, aby zobaczyć błędy
//   5. Wybierz pozycję Projekt > Dodaj nowy element, aby utworzyć nowe pliki kodu, lub wybierz pozycję Projekt > Dodaj istniejący element, aby dodać istniejące pliku kodu do projektu
//   6. Aby w przyszłości ponownie otworzyć ten projekt, przejdź do pozycji Plik > Otwórz > Projekt i wybierz plik sln
