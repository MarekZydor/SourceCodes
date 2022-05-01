#include <iostream>
#include <vector>
#include <algorithm>
#include <forward_list>

const int MAXN = 1000000;

int ceny[MAXN+1];
std::vector<int> indeksyNaDzieciRodzicaZerowego[MAXN];
int indeksyRodzicow[MAXN];
int czyUstawioneCeny[MAXN+1];
std::forward_list<int> nieprzypisaneCeny, niepotwierdzoneCeny;
std::vector<int> indeksyZRodzicemNiezerowym;
int niejednoznaczne[MAXN+1];


bool cenaRodzicowCMP(int index1, int index2)
{
    if(ceny[indeksyRodzicow[index1]] < ceny[indeksyRodzicow[index2]])
        return true;
    return false;
}

int liczIloscPotomkow(int i)
{
    int suma = 0;
    for(int j : indeksyNaDzieciRodzicaZerowego[i])
    {
        suma += 1+liczIloscPotomkow(j);
    }
    return suma;
}

void ustawCene(int index, std::vector<int> * dostepneCeny)
{
    ceny[index] = dostepneCeny->back();
    dostepneCeny->pop_back();
    if(indeksyNaDzieciRodzicaZerowego[index].size() == 1 && !dostepneCeny->empty())
        ustawCene(indeksyNaDzieciRodzicaZerowego[index][0], dostepneCeny);
}

int main() {
    std::ios_base::sync_with_stdio(false);

    int numberOfWorkers;
    std::cin>>numberOfWorkers;


    for(int i = 0; i<numberOfWorkers; i++)
    {
        int p,z;
        std::cin>>p>>z;
        ceny[i] = z;
        indeksyRodzicow[i] = p-1;
        czyUstawioneCeny[z] = 1;
        if(p-1 == i)
            ceny[i] = numberOfWorkers;
    }
    for(int i = 0; i<numberOfWorkers; i++) {
        if(ceny[indeksyRodzicow[i]] != 0 && ceny[i] == 0)
        {
            indeksyZRodzicemNiezerowym.push_back(i);
        }else if(ceny[i] == 0)
        {
            indeksyNaDzieciRodzicaZerowego[indeksyRodzicow[i]].push_back(i);
        }
    }
    std::sort(indeksyZRodzicemNiezerowym.begin(), indeksyZRodzicemNiezerowym.end(), cenaRodzicowCMP);
    for(int i = numberOfWorkers; i>0; i--)
    {
        if(czyUstawioneCeny[i] == 0)
        {
            nieprzypisaneCeny.push_front(i);
            niepotwierdzoneCeny.push_front(i);
        }
    }

    for(int i : indeksyZRodzicemNiezerowym)
    {
        int iloscPotomkow = 1+liczIloscPotomkow(i);
        std::vector<int> dostepneCeny;
        while(!nieprzypisaneCeny.empty())
        {
            int badanaCena = nieprzypisaneCeny.front();
                if(badanaCena < ceny[indeksyRodzicow[i]] && dostepneCeny.size() < iloscPotomkow)
                {
                    dostepneCeny.push_back(badanaCena);
                    nieprzypisaneCeny.pop_front();
                }else
                    break;

        }

        bool niejednoznaczna = false;
        if(nieprzypisaneCeny.empty() || nieprzypisaneCeny.front() > ceny[indeksyRodzicow[i]])
        {
            ustawCene(i,&dostepneCeny);
        }else
            niejednoznaczna = true;

        while(!niepotwierdzoneCeny.empty())
        {
            int napotkaneCeny = niepotwierdzoneCeny.front();
            if (napotkaneCeny < ceny[indeksyRodzicow[i]]) {
                if(niejednoznaczna)
                    niejednoznaczne[napotkaneCeny] = 1;
                niepotwierdzoneCeny.pop_front();
            } else
                break;
        }
    }


    for(int i = 0; i<numberOfWorkers; i++)
    {
        if(niejednoznaczne[ceny[i]] == 1)
        {
            std::cout<<0<<std::endl;
        }else
            std::cout<<ceny[i]<<std::endl;
    }

    return 0;
}