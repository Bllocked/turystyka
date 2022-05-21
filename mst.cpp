#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

class krawedzie
{
public:
    int c1;
    int c2;
    int p;
    krawedzie(int nc1, int nc2, int np)
    {
        c1 = nc1;
        c2 = nc2;
        p = np;
    }
    //zamienia wagi na przeciwne
    void odwrocenie()
    {
        int x;
        x = this->p;
        x = -x;
        this->p = x;
    }
};
//dodaje drogi
void drogi(int d, vector<krawedzie>& k)
{
    int c1, c2, p;
    for (int i = 0; i < d; i++)
    {
        cin >> c1 >> c2 >> p;
        k.push_back(krawedzie(c1, c2, p));
    }
}
//stosuje zamiane dla wszystkich obiektow
void zamiana_wagi(vector<krawedzie>& k)
{
    for (size_t i = 0; i < k.size(); i++)
    {
        k[i].odwrocenie();
    }
}
//porownuje wagi
bool porownanie(krawedzie k1, krawedzie k2)
{
    return k1.p < k2.p;
}
//sprawdza czy pod indeksem jest wpisana wartosc indeksu, jezeli tak to ja zwraca, a jezeli nie, to przechodzi do indeksu ktory byl wpisany
int znajdzMiasto(int x, int* miasta)
{
    if (miasta[x] == x)
        return x;
    return znajdzMiasto(miasta[x], miasta);
}
//dfs

void kruskal(int m, vector<krawedzie>& k, vector<krawedzie>& mst)
{
    //tablica do porownywania indeksow
    int* miasta = new int[m];
    //wypelnienie tablicy indeksami
    for (int i = 0; i < m; i++)
        miasta[i] = i;
    //licznik ilosci potrzebnych drog
    int licznik = 0;
    int i = 0;
    //miasta-1=liczba potrzebnych drog
    while (licznik < m - 1)
    {
        if (i == k.size())
            break;
        //indeksy zaczynaja sie od 0, a miasta od 1, dlatego odejmujemy 1
        int miastoPoczatkowe = znajdzMiasto(k[i].c1 - 1, miasta);
        int miastoDocelowe = znajdzMiasto(k[i].c2 - 1, miasta);
        //jezeli wartosci sa identyczne to znaczy ze istnieje polaczenie miedzy nimi
        if (miastoDocelowe != miastoPoczatkowe)
        {
            int tym;
            if (k[i].c1 > k[i].c2)
            {
                tym = k[i].c2;
                k[i].c2 = k[i].c1;
                k[i].c1 = tym;
            }
            mst.push_back(k[i]);
            tym = k[i].c1;
            k[i].c1 = k[i].c2;
            k[i].c2 = tym;
            mst.push_back(k[i]);
            licznik++;
            //wiaze miasta ze soba
            miasta[miastoPoczatkowe] = miastoDocelowe;
        }
        i++;
    }
    delete[]miasta;
    k.clear();
}
void odwiedzone(bool visited[], vector<vector<int>>& sasiedzi, vector<int>& trasa, int s, int e)
{
    visited[s] = true;
    // cout << c<<" ";
    vector<int>::iterator i;
    for (i = sasiedzi[s].begin(); i != sasiedzi[s].end(); i++)
    {
        if (!visited[*i])
        {
            if (trasa.back() == s)
            {
                trasa.pop_back();
            }
            trasa.push_back(s);
            if (*i == e)
            {
                trasa.push_back(e);
                break;
            }
            odwiedzone(visited, sasiedzi, trasa, *i, e);
        } 
    }
}
//jakie krawedzie sa przypisane do jakich
void neigh(vector<krawedzie>& mst, int m, vector<vector<int>>& sasiedzi)
{

    for (size_t i = 0; i < mst.size(); i++)
    {
        sasiedzi[mst[i].c1].push_back(mst[i].c2);
    }


}
//ustawia tabele visited na false
void wypelnianie(bool visited[], int m)
{
    for (int i = 0; i <= m; i++)
    {
        visited[i] = false;
    }
}
int liczba_pasazerow(vector<krawedzie>& mst, vector<int>& trasa)
{
    int p = 1000000;
    for (size_t i = 1; i < trasa.size() - 1; i++)
    {
        for (size_t j = 0; j < mst.size(); j++)
        {
            if (mst[j].c1 == trasa[i] && mst[j].c2 == trasa[i + 1])
            {
                if (p > mst[j].p)
                    p = mst[j].p;
            }
        }
    }
    return p;
}
int liczba_przejazdow(int t, int p)
{
    int suma = 0;
    while (t > 0)
    {
        t -= p;
        suma++;
    }
    return suma;
}
int main()
{
    vector <krawedzie> k;
    vector <krawedzie> mst;
    
    int m, d, s=-1, e=-1, p,t;
    cin >> m;
    cin >> d;
    vector<vector<int>> sasiedzi(m + 1);

    drogi(d, k);
    zamiana_wagi(k);
    sort(k.begin(), k.end(), porownanie);
    kruskal(m, k, mst);
    zamiana_wagi(mst);
    neigh(mst, m, sasiedzi);
    
    
    while (s != 0 && e != 0)
    {
        cin >> s;
        cin >> e;
        if (s == 0 && e == 0)
            break;
        cin >> t;
        vector<int> trasa;
        trasa.push_back(0);
        bool* visited = new bool[m + 1];
        wypelnianie(visited, m);
        odwiedzone(visited, sasiedzi, trasa, s, e);
        for (size_t i = 1; i < trasa.size(); i++)
        {
            cout << trasa[i] << " ";
        }
        cout << endl;
        delete[]visited;
        trasa.clear();
    }
}
