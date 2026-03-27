#pragma once
#pragma once
#include <vector>
#include <deque>
#include <random>

//
class ARX
{
private:
    std::vector<double> a;
    std::vector<double> b;
    int k;
    std::deque<double> yHist;
    std::deque<double> uHist;
    std::deque<double> kBuffer;

    double uMin, uMax;
    double yMin, yMax;
    bool stanLimitow;

    double amplitudaSzumu = 0.0;
    bool stanSzumu;
    std::mt19937 generator;
    std::normal_distribution<double> distribution;

    double generujSzum();

    double zastosujLimity(double, double, double);

public:
    ARX(std::vector<double> _a, std::vector<double> _b, int _k = 1);

    double symuluj(double);

    void ustawParametry(std::vector<double>, std::vector<double>, int);
    void ustawA(std::vector<double>);
    void ustawB(std::vector<double>);
    void ustawOpoznienie(int);
    void ustawAmplitudeSzumu(double amp);

    void ustawLimitWejscia(double, double);
    void ustawLimitWyjscia(double, double);

    void przelaczLimity(bool);
    void przelaczSzum(bool);

    std::vector<double> getA() const { return a; }
    std::vector<double> getB() const { return b; }
    int getK() const { return k; }
    double getSzum() const { return distribution.stddev(); }
    double getUMin() const { return uMin; }
    double getUMax() const { return uMax; }
    double getYMin() const { return yMin; }
    double getYMax() const { return yMax; }
    bool getLimityAktywne() const { return stanLimitow; }
    bool getSzumAktywny() const { return stanSzumu; }
};


