#include<iostream>
#include<ctime>
using namespace std;

int main()
{
	//ASM beadandóban a cél:
	//változó létrehozáson kívül minden funkcionális rész ASM blokkokban legyen megírva
	//változók létrehozása
	int N; // bekérendő háromszögek száma

	const char* beker = "Bekérendő háromszögek száma: "; //printf
	const char* elsobefogo = "Adja meg a(z) %i. háromszög első befogóját:\t"; //printf
	const char* masodikbefogo = "Adja meg a(z) %i. háromszög második befogóját:\t"; //printf
	const char* formati = "%d"; //scanf-hez
	const char* formatf = "%f"; //lebegőpontos kiíráshoz
	const char* format_scanfDouble = "%lf";
	const char* hun = "HUN"; //setlocale
	const char* _pause = "pause"; //systemhez
	const char* _kiiras_asmbol = "ASM Az %d.elem: %d\t%d\t%d\n";


	//ESP mentése
	int _saveesp;
	_asm
	{
		mov _saveesp, ESP;
	}


	_asm
	{
		//setlocale
		push hun;
		push LC_ALL;
		call dword ptr setlocale;
		//  tömb hosszának bekérése
		//kiírás
		push beker;
		call printf;
		//bekérés
		lea eax, N; //N címe eax-ben
		push eax; //verem tetejére tesszük
		push formati; //formázó karakter
		call scanf;
	}

	//ellenőrzés:
	_asm
	{
		push N;
		push formati;
		call dword ptr printf;
	}
	cout << endl << N << " elemu tombok letrehozasa...\n";
	double* A = new double[N];
	double* B = new double[N];
	double* CC = new double[N];
	int min_atmero = 0, max_atmero = 0;
	int haromszogszam = 1;
	int tombindex; //asm tömbök indexelése (nem akarunk szorozni)
	const char* tombelem = "%d\t";
	const char* ujsor = "\n";
	const char* tombszoveg = "A(z) %d elemű tömb tartalma: \n";
	const char* haromszogbe = "\nHarmoszog adatai: %f\t%f\n";
	const char* szorzas_erdmenye = "\n!!C negyzet: %f\n";
	int mentsukmegazespt;
	int asd = 2;
	double cNegyzet;
	// tömb feltöltése, adatok bekérése
	_asm {
		//fejléc
		push N;
		push tombszoveg;
		call printf;
		//tömbelemek kiírása:
		mov ecx, N;
		mov tombindex, 0;
	ciklus_befogo_beker:
		push ecx; //ecx menteni kell
		mov edi, esp; //esp mentése

		push tombindex;
		push formati;
		call dword ptr printf;

		//elso befogo bekerese, szoveg kiirasa
		push haromszogszam;
		push elsobefogo;
		call printf;
		//elso befogo bekerese, scanf fuggveny
		mov eax, dword ptr[A]; //a címe eax-ben
		add eax, tombindex;
		push eax; //verem tetejére tesszük
		push format_scanfDouble; //formázó karakter
		call dword ptr scanf;



		//masodik befogo bekerese, szoveg kiirasa
		push haromszogszam;
		push masodikbefogo;
		call printf;
		//masodik befogo bekerese, scanf fuggveny
		mov eax, dword ptr[B]; //b címe eax-ben
		add eax, tombindex;
		push eax; //verem tetejére tesszük
		push format_scanfDouble; //formázó karakter
		call dword ptr scanf;

		mov esp, edi; //esp vissza
		pop ecx;
		add tombindex, 8;
		add haromszogszam, 1;



		loop ciklus_befogo_beker;
		//újsor jel:
		push ujsor;
		call printf;

	}

	_asm {
		// C - oldal szamitasa
		mov ecx, N;
		mov tombindex, 0;
	atfogo_szamitasa:
		push ecx; //ecx menteni kell
		mov edi, esp; //esp mentése


		mov eax, dword ptr[A]; // A cime másolása eax-ba
		add eax, tombindex;		// eltolás tombindex-el
		fldz; // 0.0 a verem tetejere
		fadd qword ptr[eax]; // a[i] a verem tetejen
		fmul qword ptr[eax]; // a*a a verem tetejen

		//fstp cNegyzet;

		mov eax, dword ptr[B]; // B cime másolása eax-ba
		add eax, tombindex;		// eltolás tombindex-el
		fldz; // 0.0 a verem tetejere
		fadd qword ptr[eax]; // B[i] a verem tetejen
		fmul qword ptr[eax]; // b*b a verem tetejen

		faddp st(1), st(0);
		fsqrt;


		mov eax, dword ptr[CC]; // A cime másolása eax-ba
		add eax, tombindex;		// eltolás tombindex-el
		fstp qword ptr[eax];




		//push qword ptr[eax]; //printf-hez
		//push szorzas_erdmenye;
		//call dword ptr printf;


		mov esp, edi; //esp vissza
		pop ecx;
		add tombindex, 8;

		loop atfogo_szamitasa;


	}


	cout << "Forral: " << endl;
	cout << "kiiratas_forral:" << endl;
	for (int i = 0; i < haromszogszam - 1; i++)
	{
		cout << i << ".elem " << A[i] << " " << B[i] << " " << CC[i] << endl;
	}
	//cout << "Cnegyzet:" << cNegyzet << endl;


	


	int minimum, maximum;
	double kuka;
	// min max kereses
	_asm {
		mov minimum, 0;
		mov maximum, 0;
		mov ecx, N;
		mov tombindex, 0;


	min_max_kereses:
		push ecx; //ecx menteni kell
		mov edi, esp; //esp mentése




		mov eax, dword ptr[CC]; // A cime másolása eax-ba
		add eax, minimum;		// eltolás minimummal-el
		mov eax, dword ptr[eax];	//cim forditasa ertekke
		fld qword ptr[eax];					// minimum ertek a verem tetejen


		mov eax, dword ptr[CC]; // A cime másolása eax-ba
		add eax, tombindex;		// eltolás minimummal-el
		mov eax, dword ptr[eax];	//cim forditasa ertekke
		fld qword ptr[eax];					// vizsgalando ertek a verem tetejen

		fstp kuka;
		fstp kuka;



		mov esp, edi; //esp vissza
		pop ecx;
		add tombindex, 4;
		loop min_max_kereses;
	}



	delete[] A;
	delete[] B;
	delete[] CC;

	//system("pause");
	_asm
	{
		push _pause;
		call dword ptr system;
	}
	//ESP visszaállítás:
	_asm
	{
		mov ESP, _saveesp;
	}
	
	return 0;
}