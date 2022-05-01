#include <iostream>
#include "lista_jed.h"
#include "Graham.h"


void import_from_to(const char *string, Graham *graham);


double dist(point * a, point * b)
{
    double xLength = a->x - b->x;
    double yLength = a->y - b->y;
    return xLength * xLength + yLength * yLength;
}


double cmp(point * a, point * b, point * c)
{
    return (b->y - a->y) * (c->x - b->x) - (b->x - a->x) * (c->y - b->y);
}

bool cmp_for_sort(point * startpoint, point * b, point * c)
{
    double mult = cmp(startpoint, b, c);
    if(mult == 0)
    {
        if (dist(startpoint, c) < dist(startpoint, b))
        {
            return true;
        }
    } else if (mult > 0)
    {
        return true;
    }
    return false;
}


int main() {
    Graham ob_g;
    import_from_to("points4.txt", &ob_g);

    ob_g.setStartPoint();

    auto t1 = std::chrono::high_resolution_clock::now();
    ob_g.sort(cmp_for_sort);
    auto t2 = std::chrono::high_resolution_clock::now();

    auto timeSort = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

    t1 = std::chrono::high_resolution_clock::now();
    ob_g.graham_scan(cmp);
    t2 = std::chrono::high_resolution_clock::now();

    auto timeLoop = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout<< ob_g.print_data(timeSort, timeLoop);

    ob_g.print_graph();

    ob_g.clear();

    return 0;
}

void import_from_to(const char *string, Graham * graham) {
    std::ifstream file;
    file.open(string);
    if(!file)
    {
        std::cerr<<"Nie mozna takiego pliku otworzyc";
        exit(1);
    }
    int n;
    file>>n;
    point ** points = new point*[n];

    for(int i = 0; i<n; i++)
    {
        double x,y;
        file>>x>>y;
        points[i] = new point(x,y);
    }
    *graham = {points, n};
    file.close();
}



#ifndef GRAHAM_H
#define GRAHAM_H
#include "gnuplot-iostream.h"
#include "HeapSort.h"

template<typename T> std::string toStr(const T& t)
{
    std::ostringstream os;
    os << t;
    return os.str();
}



struct Point
{
    double x, y;
    Point(double d, double d1) : x(d), y(d1) {}
    bool operator==(Point &lhs) const
    {
        return (x == lhs.x && y == lhs.y);
    }
};


class Graham{
private:
    Point ** points;    //points array that will be sort
    Point ** pointsOrg; //points array orginal for result
    Point * startPoint;
    int countPoints;
public:
    int getCountPoints() const {
        return countPoints;
    }

private:
    list<int> indexPointsResult;
public:
    Graham() {}
    Graham(Point ** pPoint, int i) {
        pointsOrg = pPoint;
        countPoints = i;
        points = new Point*[i];
        for(int j = 0;j<i;j++)
        {
            points[j] = new Point(pPoint[j]->x, pPoint[j]->y);
        }
    }

    void print_graph()
    {
        Gnuplot gp(R"("C:\Program Files\gnuplot\bin\gnuplot.exe" -persist)");
        gp << "$edge << EOD\n";
        for(int i = 0; i < indexPointsResult.getSize(); i++) {
            gp << toStr(points[indexPointsResult[i]]->x) << " "
               << toStr(points[indexPointsResult[i]]->y) << " ";
            if(i != indexPointsResult.getSize()-1) {
                gp << toStr(points[indexPointsResult[i + 1]]->x) << " "
                   << toStr(points[indexPointsResult[i + 1]]->y) << "\n";
            }else
            {
                gp << toStr(points[indexPointsResult[0]]->x) << " "
                   << toStr(points[indexPointsResult[0]]->y) << "\n";
            }
        }gp << "EOD\n";
        gp << "plot '$edge' using 1:2:($3-$1):($4-$2) with vectors lw 2 notitle\n";

        gp << "$data << EOD\n";
        for(int i = 0; i < countPoints; i++)
            gp << toStr(pointsOrg[i]->x) << " " << toStr(pointsOrg[i]->y) << " " << toStr(i) << "\n";
        gp << "EOD\n";
        gp << "replot '$data' using 1:2:3 with labels point pt 7 offset char 0.6,0.6 title 'ID punktow'\n";

        gp << "$dataStart << EOD\n";
        gp << toStr(startPoint->x) << " " << toStr(startPoint->y) << "\n";
        gp << "EOD\n";
        gp << "replot '$dataStart' using 1:2 with points pt 6 ps 7 notitle\n";
    }

    void setStartPoint() {
        int indeksStart = 0;
        startPoint = points[indeksStart];

        for(int i = 1; i<countPoints;i++)
        {
            if((startPoint->y > points[i]->y) || (startPoint->y == points[i]->y && startPoint->x > points[i]->x))
            {
                indeksStart = i;
                startPoint = points[indeksStart];
            }
        }
        //zamiana pozycji pierwszego elementu tablicy z startpointem
        Point * tmp = points[indeksStart];
        points[indeksStart] = points[0];
        points[0] = tmp;
    }

    void sort(bool(*cmp)(Point *, Point *, Point *)) {
        HeapSort<Point *>::sort(points,countPoints,cmp,startPoint);
    }

    Point *& operator[](int index) noexcept(false)
    {
        if(index < 0 || index >= countPoints)
        {
            throw std::out_of_range("Podano liczbÄ™ spoza zakresu");
        }else{
            return points[index];
        }

    }

    void graham_scan(double (*cmp)(Point *, Point *, Point *)) {
        indexPointsResult.dodaj_koniec(0);
        indexPointsResult.dodaj_koniec(1);
        int i0, i1, i2;
        for(int i = 2; i<countPoints; i++)
        {
            indexPointsResult.dodaj_koniec(i);

            int sizeCurr = indexPointsResult.getSize();
            i0 = indexPointsResult[sizeCurr - 3];
            i1 = indexPointsResult[sizeCurr-2];
            i2 = indexPointsResult[sizeCurr-1];

            while(cmp(points[i0], points[i1], points[i2]) >= 0.0)
            {
                indexPointsResult.usun_przedostatni();
                sizeCurr = indexPointsResult.getSize();
                if(sizeCurr == 2) break;
                i0 = indexPointsResult[sizeCurr-3];
                i1 = indexPointsResult[sizeCurr-2];
                i2 = indexPointsResult[sizeCurr-1];
            }

        }
    }

    std::string print_data(long long int timeSort, long long int timeLoop) {
        std::string res = "";
        res += "Ilosc punktow w powloce wypuklej: "+ toStr(indexPointsResult.getSize())+"\n";
        res += "Indeksy:\n";
        for(int i = 0; i<indexPointsResult.getSize(); i++)
        {
            for(int j = 0; j<countPoints; j++)
            {
                if(*points[indexPointsResult[i]] == *pointsOrg[j])
                {
                    res += toStr(j) + " ";
                    break;
                }
            }

        }
        res += "\nCzas obliczen:\nSortowanie: "+toStr(timeSort)+"ms\nPetla: "+toStr(timeLoop)+"us\n";

        return res;
    }
    void clear()
    {
        for(int i = 0; i<countPoints; i++)
        {
            delete points[i];
            delete pointsOrg[i];
        }
        delete [] points;
        delete [] pointsOrg;
    }
};

#endif //GRAHAM_H


