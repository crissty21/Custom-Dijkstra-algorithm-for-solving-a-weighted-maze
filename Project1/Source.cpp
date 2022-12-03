#include "MyForm.h"
#include <windows.h>
#include <fstream>


using namespace Project1;

int mutari[4][1000], k = 1;
void verificare();
//transpunem labiritul intr-un graf
//clasa nodurilor 
//fiecare patratel va reprezenta un nod
class node {
public:
	//penalizarea aplicata pe aceasta casuta
	int penalitate = 0;
	//distanta dintre tile si start
	//daca distanta dintre start si patratelul curent este infinit, inseamna ca nu a fost calculata inca
	int distanta_start = INT_MAX;
	//numarul de casute adiacente
	//teleportul are "un vecin" in plus
	int numar_vecini;
	//referinte la casutele vecine
	node* vecini[6];


	//vecinul care duce spre start
	node* tata;

	//daca casuta curenta este sau nu teleport
	bool teleport = false;

	//coordonatele tile-ului 
	//indici pe care ii vom folosi la afisarea drumului final
	int indice_x, indice_y;

	//toti vecinii pot fi tati
	//alegem tatal cu distanta pana la start cea mai mica
	int alege_tata()
	{
		//in cea mai mare parte din cazuri, primul vecin are o distanta pana la start cunoscuta
		//vom incepe cu el
		int tatal_favorit = vecini[1]->distanta_start;
		tata = vecini[1];

		//daca ne aflam pe un teleport, vom alege dintre toti vecinii distanta cea mai mica
		if (teleport)
		{
			//alegem tatal cu distanta pana la start minima
			for (int i = 2; i <= numar_vecini; i++)
				if (vecini[i]->distanta_start < tatal_favorit)
				{
					//salvam distanta pana la start a tatalui preferan
					tatal_favorit = vecini[i]->distanta_start;
					//salvam tatal pregerat
					tata = vecini[i];
				}
		}
		//daca nu ne aflam, trebuie sa avem grija la un posibil vecin teleport
		else
		{
			//parcurgem toti vecinii
			for (int i = 2; i <= numar_vecini; i++)
				if (vecini[i]->distanta_start < tatal_favorit)
				{
					//daca avem un vecin teleport, il putem folosi pe post de tata, doar daca acesta la randul sau are tata pe celalalt teleport
					if (vecini[i]->teleport)
					{
						if (vecini[i]->tata->teleport)
						{
							//salvam distanta pana la start a tatalui preferan
							tatal_favorit = vecini[i]->distanta_start;
							//salvam tatal preferat
							tata = vecini[i];
						}
					}
					else
					{
						//salvam distanta pana la start a tatalui preferan
						tatal_favorit = vecini[i]->distanta_start;
						//salvam tatal preferat
						tata = vecini[i];
					}
				}
		}
		//Returnam distanta optima pana la start

		//daca nu am gasit un tata cu o distanta pana la start finita, ne vom asigura ca returnam o distanta care sa nu distruga programul :)) - hope that makes sens
		if (tatal_favorit == INT_MAX)
		{
			//in functia rezolvare, facem adunarea dintre valoare din tatal_favorit si penalitate. Daca tatal favorit retine INT_MAX, iar noi ii adunam ceva, il dam peste cap :)
			return tatal_favorit - penalitate;
		}
		return tatal_favorit;
	}
	//functie responsabila de actualizarea copiilor 
	void actualizare_copii()
	{
		//daca ne aflam pe teleport, verificam daca tatal sau este tot teleport sau nu 
		//daca este, atunci actualizam vecinii, iar daca nu, actualizam cel de al doilea teleport
		if (teleport)
		{
			if (tata->teleport)
			{
				//daca ne-am teleportat aici, avem voie sa actualizam vecinii, indiferent daca sunt finiti sau nu 
				//nu luam in calcul si ultimul vecin, care este teleportul
				for (int i = 1; i < numar_vecini; i++)
				{
					if (vecini[i]->distanta_start > vecini[i]->penalitate + distanta_start)
					{
						//actualizam atat distanta pana la start, cat si tatal
						vecini[i]->distanta_start = vecini[i]->penalitate + distanta_start;
						mutari[1][k] = vecini[i]->indice_x;
						mutari[2][k] = vecini[i]->indice_y;
						mutari[3][k] = vecini[i]->distanta_start;
						k++;
						vecini[i]->tata = this;
						//actualizam copiii copilului
						vecini[i]->actualizare_copii();
					}
				}
			}
			else
			{
				//daca tatal teleportului nu este cel de al doilea teleport, inseamna ca am ajuns aici de pe o casuta adiacenta
				//fapt ce ne spune ca singura casuta unde putem merge este celalalt teleport
				if (vecini[numar_vecini]->distanta_start >= distanta_start)
				{
					//daca il modificam, ii actualizam copiii
					vecini[numar_vecini]->distanta_start = distanta_start;
					vecini[numar_vecini]->tata = this;
					mutari[1][k] = vecini[numar_vecini]->indice_x;
					mutari[2][k] = vecini[numar_vecini]->indice_y;
					mutari[3][k] = vecini[numar_vecini]->distanta_start;
					k++;
					vecini[numar_vecini]->actualizare_copii();
				}
			}
		}
		else
		{
			//daca nu este teleport, verificat distanta pana la start a tuturor vecinilor
			for (int i = 1; i <= numar_vecini; i++)
			{
				//daca copiii au o distanta pana la start diferita de int_max, inseamna ca am mai trecut pe la ei
				//fapt ce determina actualizarea distantei 
				//aceasta conditie asigura ca nu facem un numar de miscari de ordinul miilor
				if (vecini[i]->distanta_start != INT_MAX && vecini[i]->distanta_start > vecini[i]->penalitate + distanta_start)
				{
					vecini[i]->distanta_start = vecini[i]->penalitate + distanta_start;
					vecini[i]->tata = this;
					mutari[1][k] = vecini[numar_vecini]->indice_x;
					mutari[2][k] = vecini[numar_vecini]->indice_y;
					mutari[3][k] = vecini[i]->distanta_start;
					k++;
					vecini[i]->actualizare_copii();
				}
			}
		}
	}
};
ifstream lab("date.in");
node* start, * finish;

node* graf[12][12];
int inaltime = 11, latime = 11;
//functie responsabila de citirea datelor din fisier
void citire()
{
	//retine unul dintre teleporturi
	node* gateway = NULL;
	//contor pentru memorarea numarului de vecini
	int k;

	//cream matricea nodurilor
	for (int i = 1; i <= inaltime; i++)
		for (int j = 1; j <= latime; j++)
			graf[i][j] = new node();

	//citirea grafului
	for (int i = 1; i <= inaltime; i++)
		for (int j = 1; j <= latime; j++)
		{
			lab >> graf[i][j]->penalitate;
			//salvam indicii din matrice;
			graf[i][j]->indice_x = i;
			graf[i][j]->indice_y = j;


			k = 1;
			//adaugam dreapta
			if (j < latime)
				graf[i][j]->vecini[k++] = graf[i][j + 1];
			//adaugam jos
			if (i < inaltime)
				graf[i][j]->vecini[k++] = graf[i + 1][j];
			//adaugam stanga
			if (i > 1)
				graf[i][j]->vecini[k++] = graf[i - 1][j];
			//adaugam sus
			if (j > 1)
				graf[i][j]->vecini[k++] = graf[i][j - 1];

			graf[i][j]->numar_vecini = k - 1;

			//valoarea de -1 dicteaza faptul ca nodul este un teleport
			if (graf[i][j]->penalitate == -1)
			{
				//in cazul teleportului, penalitatea este 0
				graf[i][j]->penalitate = 0;
				//marcam tile-ul ca fiind teleport
				graf[i][j]->teleport = true;
				//salvam legatura cu al doilea teleport, daca acesta este descoperit
				graf[i][j]->numar_vecini++;
				//daca nu se cunoaste al doilea teleport, retinem teleportul curent
				if (gateway == NULL)
					gateway = graf[i][j];
				//in schimb, daca il cunoastel, le legam
				else
				{
					graf[i][j]->vecini[graf[i][j]->numar_vecini] = gateway;
					gateway->vecini[gateway->numar_vecini] = graf[i][j];
				}
			}
		}
	//distanta dintre primul tile si start este 0 - logic dăăă
	graf[1][1]->distanta_start = 0;
	//salvam startul si finish-ul
	start = graf[1][1];
	finish = graf[inaltime][latime];

}
//functia principala de rezolvare
void rezolvare()
{
	//se pleaca de la primul element din matrice
	for (int i = 1; i <= inaltime; i++)
		for (int j = 1; j <= latime; j++)
		{
			mutari[1][k] = graf[i][j]->indice_x;
			mutari[2][k] = graf[i][j]->indice_y;
			//ignoram primul element, deoarece acesta nu are tati
			if (j != 1 || i != 1)
			{
				//apelam fuctia alege_tata(), care ne va returna distanta minima pana la start a tatilor compatibili
				graf[i][j]->distanta_start = graf[i][j]->alege_tata() + graf[i][j]->penalitate;
				mutari[3][k] = graf[i][j]->distanta_start;
				k++;
				//daca modificam valoarea, actualizam copiii
				graf[i][j]->actualizare_copii();
			}
			else
			{
				mutari[3][k] = 0;
				k++;
			}
		}
	verificare();
}
//functie care se asigura ca nu mai avem valori infinite dupa rezolvarea initiala a labirintului
void verificare()
{
	//se pleaca de la primul element din matrice
	for (int i = 1; i <= inaltime; i++)
		for (int j = 1; j <= latime; j++)
		{
			//daca se descopeta un element cu distanta pana la start infinita, i se alege un tata, si i se modifica aceasta distanta 
			if (graf[i][j]->distanta_start == INT_MAX)
			{
				mutari[1][k] = graf[i][j]->indice_x;
				mutari[2][k] = graf[i][j]->indice_y;
				graf[i][j]->distanta_start = graf[i][j]->alege_tata() + graf[i][j]->penalitate;
				mutari[3][k] = graf[i][j]->distanta_start;
				k++;
				//ii actualizam copiii 
				graf[i][j]->actualizare_copii();
			}
		}
}


[STAThreadAttribute]

int main() {

	citire();
	rezolvare();
	MyForm mf;
	int a, numar_elemente_drumuri[12][12] = { 1 };
	node* aux;
	for(int i=1;i<=11;i++)
		for (int j = 1; j <= 11; j++)
		{
			if (graf[i][j]->teleport)
				mf.b[i, j] = -1;
			else
				mf.b[i, j] = graf[i][j]->penalitate;
			aux = graf[i][j];
			while (aux != start)
			{
				mf.drumuri[i, j, numar_elemente_drumuri[i][j], 1] = aux->indice_x;
				mf.drumuri[i, j, numar_elemente_drumuri[i][j], 2] = aux->indice_y;
				numar_elemente_drumuri[i][j]++;
				aux = aux->tata;
			}
			mf.drumuri[i, j, numar_elemente_drumuri[i][j], 1] = start->indice_x;
			mf.drumuri[i, j, numar_elemente_drumuri[i][j], 2] = start->indice_y;
			mf.drumuri[i, j, 0, 0] = numar_elemente_drumuri[i][j];
		}
	for (int i = 1; i < k; i++)
	{
		mf.coordonate[1, i] = mutari[1][i];
		mf.coordonate[2, i] = mutari[2][i];
		mf.coordonate[3, i] = mutari[3][i];
		cout << mutari[1][i] << " " << mutari[2][i] << endl;
	}
	mf.limita = k - 1;
	mf.desenare();
	mf.ShowDialog();
	return 0;
}