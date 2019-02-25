#include <iostream>
#include <cmath>

using namespace std;


//klasa Macierz i jej metody
class Macierz
{
	private:
		int wier; //ilosc wierszy	wier x kol
		int kol; //ilosc kolumn    
		double **tablica;
	
	public:
		Macierz(int n, int m);
		int wysokosc();
		int szerokosc();
		double uzyskaj_wartosc(int n, int m);
		void zmien_wartosc(int n, int m, double liczba);
		void mnozenie_liczba(int k);
};


Macierz::Macierz(int n, int m)
{
	wier = n;
	kol = m;
	
	tablica = new double* [wier];	//generowanie macierzy
	for (int i = 0; i < kol; i++)
	{
		tablica[i] = new double [kol];
	}
	
	for (int i = 0; i < kol; i++)	//zape³nianie macierzy zerami
	{
		for (int j = 0; j < wier; j++)
		{
			tablica[i][j] = 0;
		}
	}
}

int Macierz::wysokosc()
{
	return wier;
}

int Macierz::szerokosc()
{
	return kol;
}

double Macierz::uzyskaj_wartosc(int n, int m)
{
	return tablica[n][m];
}
	
void Macierz::zmien_wartosc(int n, int m, double liczba)
{
	tablica[n][m] = liczba;
}


void Macierz::mnozenie_liczba(int liczba)
{
	for (int i = 0; i < kol; i++)
	{
		for (int j = 0; j < wier; j++)
		{
			tablica[i][j] = liczba * tablica[i][j];
		}
	}
}





//Funkcje globalne

void mnozenie_macierzy(Macierz macierz1, Macierz macierz2)
{
	if(macierz1.szerokosc() != macierz2.wysokosc())
	{
		cout << "Nie mozna pomnozyc macierzy" << endl;
		system("pause");
		return;
	}
	else
	{
		int n = macierz1.wysokosc();
		int m = macierz2.szerokosc();
		Macierz wynik(n, m);
		
		for(int i = 0; i < n; i++)
		{
			for(int j = 0; j < m; j++)
			{
				double suma = 0;
				for(int k = 0; k < m; k++)
				{
					suma += macierz1.uzyskaj_wartosc(k, i) * macierz2.uzyskaj_wartosc(j, k);
				}
				wynik.zmien_wartosc(j, i, suma);
			}
		}
	}	
}

double calkowanie_trapezy(Macierz funkcja)
{
	double wynik;
	double h = 0.0001; //d³ugosc kroku
	for (int i = 1; i < funkcja.szerokosc(); i++)
	{
		wynik += (funkcja.uzyskaj_wartosc(i-1,0) + funkcja.uzyskaj_wartosc(i,0)) * h / 2;
	}
}


int main(int argc, char** argv) {
	Macierz x(3, 3);
	x.zmien_wartosc(1,2,5);
	cout << x.uzyskaj_wartosc(1,0) << " " << x.uzyskaj_wartosc(1,1) << " " << x.uzyskaj_wartosc(1,2) << endl;
	
	system("pause");
	return 0;
}
