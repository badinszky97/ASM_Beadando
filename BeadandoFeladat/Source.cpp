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
	const char* hun = "HUN"; //setlocale
	const char* _pause = "pause"; //systemhez


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
	int* A = new int[N];
	int* B = new int[N];
	int* C = new int[N];
	int min_atmero = 0, max_atmero = 0;
	int haromszogszam = 1;
	int tombindex; //asm tömbök indexelése (nem akarunk szorozni)
	const char* tombelem = "%d\t";
	const char* ujsor = "\n";
	const char* tombszoveg = "A(z) %d elemű tömb tartalma: \n";
	const char* haromszogbe = "\nHarmoszog adatai: %f\t%f\n";
	const char* szorzas_erdmenye = "\nSzorzas eredmenye: %d\n";
	int mentsukmegazespt;
	int asd = 2;
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
		push formati; //formázó karakter
		call dword ptr scanf;



		//masodik befogo bekerese, szoveg kiirasa
		push haromszogszam;
		push masodikbefogo;
		call printf;
		//masodik befogo bekerese, scanf fuggveny
		mov eax, dword ptr[B]; //b címe eax-ben
		add eax, tombindex;
		push eax; //verem tetejére tesszük
		push formati; //formázó karakter
		call dword ptr scanf;

		mov esp, edi; //esp vissza
		pop ecx;
		add tombindex, 4;
		add haromszogszam, 1;



		loop ciklus_befogo_beker;
		//újsor jel:
		push ujsor;
		call printf;

	}


	cout << endl << endl;
	for (int i = 0; i < haromszogszam - 1; i++)
	{
		cout << i << ".elem " << A[i] << " " << B[i] << endl;
	}


	_asm {
		// C - oldal szamitasa
		mov ecx, N;
		mov tombindex, 0;
	atfogo_szamitas:
		push ecx; //ecx menteni kell
		mov edi, esp; //esp mentése


		mov eax, dword ptr[A];
		add eax, tombindex;
		mul asd;



		push eax;
		push szorzas_erdmenye;
		call dword ptr printf;


		mov esp, edi; //esp vissza
		pop ecx;
		add tombindex, 4;

		loop atfogo_szamitas;



	}


	delete[] A;
	delete[] B;
	delete[] C;

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