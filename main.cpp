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
		Macierz(int wiersz, int kolumna);
		int wysokosc();
		int szerokosc();
		double uzyskaj_wartosc(int wiersz, int kolumna);
		void zmien_wartosc(int wiersz, int kolumna, double liczba);
		void mnozenie_liczba(int liczba);
		void wypisz_macierz();
};


Macierz::Macierz(int wiersz, int kolumna)
{
	wier = wiersz;
	kol = kolumna;

	tablica = new double* [wier];	//generowanie macierzy
	for (int i = 0; i < wier; i++)
	{
		tablica[i] = new double [kol];
	}
	
	for (int i = 0; i < wier; i++)	//zapełnianie macierzy zerami
	{
		for (int j = 0; j < kol; j++)
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

double Macierz::uzyskaj_wartosc(int wiersz, int kolumna)
{
	return tablica[wiersz][kolumna];
}
	
void Macierz::zmien_wartosc(int wiersz, int kolumna, double liczba)
{
	tablica[wiersz][kolumna] = liczba;
}


void Macierz::mnozenie_liczba(int liczba)
{
	for (int i = 0; i < wier; i++)
	{
		for (int j = 0; j < kol; j++)
		{
			tablica[i][j] = liczba * tablica[i][j];
		}
	}
}

void Macierz::wypisz_macierz()
{
	for (int i = 0; i < wier; i++)
	{
		cout << char(179);
		for (int j = 0; j < kol; j++)
		{
			cout << tablica[i][j];
			if(j != kol - 1)
				cout << " ";
		}
		cout << char(179) << endl;
	}
	cout << endl;
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
		
		int l = macierz1.szerokosc();
		
		for(int i = 0; i < n; i++)
		{
			for(int j = 0; j < m; j++)
			{
				double suma = 0;
				for(int k = 0; k < l; k++)
				{
					suma += macierz1.uzyskaj_wartosc(i, k) * macierz2.uzyskaj_wartosc(k, j);
				}
				wynik.zmien_wartosc(i, j, suma);
			}
		}
		wynik.wypisz_macierz();	
	}
}

double calkowanie_trapezy(Macierz funkcja)
{
	double wynik;
	double h = 0.0001; //długosc kroku
	for (int i = 1; i < funkcja.szerokosc(); i++)
	{
		wynik += (funkcja.uzyskaj_wartosc(i-1,0) + funkcja.uzyskaj_wartosc(i,0)) * h / 2;
	}
}



//main
int main(int argc, char** argv) {
	Macierz x1(1, 2);
	x1.zmien_wartosc(0,0,2);
	x1.zmien_wartosc(0,1,1);
	
	Macierz x2(2, 1);
	x2.zmien_wartosc(0,0,4);
	x2.zmien_wartosc(1,0,3);

	cout << "|X1|: " << endl;
	x1.wypisz_macierz();
	cout << "|X2|: " << endl;
	x2.wypisz_macierz();
	cout << "|X1|*|X2|: " << endl;
	mnozenie_macierzy(x1, x2);
	cout << "|X2|*|X1|: " << endl;
	mnozenie_macierzy(x2, x1);
	
	
	system("pause");
	return 0;
}
