#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <algorithm>
#include <numeric>

using namespace std;

// Clase si obiecte
class Angajat
{
    // incapsulare
protected:
    string nume;
    string functie;
    int oraInceput;
    int oraSfarsit;

    // constructori si destructori
public:
    Angajat(const string &nume, const string &functie, int oraInceput, int oraSfarsit)
        : nume(nume), functie(functie), oraInceput(oraInceput), oraSfarsit(oraSfarsit) {}

    virtual ~Angajat() {}

    virtual void afiseazaInformatii() const // abstractizare
    {
        cout << "Nume: " << nume << ", Functie: " << functie
             << ", Program: " << oraInceput << "-" << oraSfarsit << "\n";
    }
};

// mostenire
class Barista : public Angajat
{
public:
    Barista(const string &nume, int oraInceput, int oraSfarsit)
        : Angajat(nume, "Barista", oraInceput, oraSfarsit) {}

    void afiseazaInformatii() const override // polimorfism - Clasa Angajat definește metoda virtuală afiseazaInformatii, iar clasa derivată Barista o suprascrie.
    {
        Angajat::afiseazaInformatii();
    }
};

class Manager : public Angajat
{
public:
    Manager(const string &nume, int oraInceput, int oraSfarsit)
        : Angajat(nume, "Manager", oraInceput, oraSfarsit) {}

    void afiseazaInformatii() const override
    {
        Angajat::afiseazaInformatii();
    }
};

class Ospatar : public Angajat
{
public:
    Ospatar(const string &nume, int oraInceput, int oraSfarsit)
        : Angajat(nume, "Ospatar", oraInceput, oraSfarsit) {}

    void afiseazaInformatii() const override
    {
        Angajat::afiseazaInformatii();
    }
};

// design pattern
class AngajatFactory
{
public:
    static Angajat *createAngajat(const std::string &tip, const std::string &nume, int oraInceput, int oraSfarsit)
    {
        if (tip == "Barista")
            return new Barista(nume, oraInceput, oraSfarsit);
        if (tip == "Manager")
            return new Manager(nume, oraInceput, oraSfarsit);
        if (tip == "Ospatar")
            return new Ospatar(nume, oraInceput, oraSfarsit);
        return nullptr;
    }
};

// Clasa pentru produse
class Produs
{
private:
    string nume;
    double pret;

public:
    Produs(const string &nume, double pret) : nume(nume), pret(pret) {}

    const string &getNume() const { return nume; }
    double getPret() const { return pret; }
};

// Clasa pentru gestionarea comenzilor
class Comanda
{
private:
    vector<Produs> produse;
    string numeClient;

public:
    Comanda(const string &numeClient) : numeClient(numeClient) {}

    void adaugaProdus(const Produs &produs)
    {
        produse.push_back(produs);
    }

    double calculeazaTotal() const
    {
        double total = 0.0;
        for (const auto &produs : produse)
        {
            total += produs.getPret();
        }
        return total;
    }

    void afiseazaComanda() const
    {
        cout << "Comanda pentru client: " << numeClient << "\n";
        for (const auto &produs : produse)
        {
            cout << " - " << produs.getNume() << ": " << produs.getPret() << " RON\n";
        }
        cout << "Total: " << calculeazaTotal() << " RON\n";
    }
};

// Clasa pentru evenimente speciale
class Eveniment
{
private:
    string nume;
    string data;
    double cost;

public:
    Eveniment(const string &nume, const string &data, double cost)
        : nume(nume), data(data), cost(cost) {}

    void afiseazaEveniment() const
    {
        cout << "Eveniment: " << nume << ", Data: " << data << ", Cost: " << cost << " RON\n";
    }
};

// Clasa pentru rapoarte financiare
class RaportFinanciar
{
private:
    double venituri;
    double cheltuieli;
    double profit;

    // template
    template <typename T>
    T calculeazaProfit(const T &venituri, const T &cheltuieli)
    {
        return venituri - cheltuieli;
    }

public:
    RaportFinanciar(double venituri, double cheltuieli)
        : venituri(venituri), cheltuieli(cheltuieli), profit(calculeazaProfit(venituri, cheltuieli)) {}

    void afiseazaRaport() const
    {
        cout << "Venituri: " << venituri << " RON, Cheltuieli: " << cheltuieli
             << " RON, Profit: " << profit << " RON\n";
    }
};

// Functii utilitare pentru manipularea fisierelor CSV
vector<vector<string>> citesteCSV(const string &numeFisier)
{
    ifstream fisier(numeFisier);
    vector<vector<string>> continut;
    string linie;

    if (!fisier.is_open())
    {
        throw runtime_error("Nu s-a putut deschide fisierul " + numeFisier);
    }

    while (getline(fisier, linie))
    {
        stringstream ss(linie);
        vector<string> rand;
        string valoare;
        while (getline(ss, valoare, ','))
        {
            rand.push_back(valoare);
        }
        continut.push_back(rand);
    }
    return continut;
}

void scrieCSV(const string &numeFisier, const vector<vector<string>> &continut)
{
    ofstream fisier(numeFisier);

    if (!fisier.is_open())
    {
        throw runtime_error("Nu s-a putut deschide fisierul " + numeFisier);
    }

    for (const auto &rand : continut)
    {
        for (size_t i = 0; i < rand.size(); ++i)
        {
            fisier << rand[i];
            if (i < rand.size() - 1)
            {
                fisier << ',';
            }
        }
        fisier << '\n';
    }
}

void adaugaAngajat(const string &numeFisier, const vector<string> &angajat)
{
    auto angajati = citesteCSV(numeFisier);
    angajati.push_back(angajat);
    scrieCSV(numeFisier, angajati);
}

void stergeAngajat(const string &numeFisier, const string &nume)
{
    // Citim conținutul CSV-ului
    auto angajati = citesteCSV(numeFisier);
    vector<vector<string>> angajatiActualizati;

    bool sters = false; // Marcăm dacă un angajat a fost șters

    for (const auto &rand : angajati)
    {
        // Comparam numele exact și ignorăm spațiile suplimentare
        if (rand[0] == nume)
        {
            sters = true;
        }
        else
        {
            angajatiActualizati.push_back(rand);
        }
    }

    if (sters)
    {
        // Rescriem fișierul cu datele actualizate
        scrieCSV(numeFisier, angajatiActualizati);
        cout << "Randul cu numele \"" << nume << "\" a fost sters din fisier.\n";
    }
    else
    {
        cout << "Nu a fost gasit un angajat cu numele \"" << nume << "\" in fisier.\n";
    }
}

int main()
{
    static double profitZilnic = 0.0; // Variabilă pentru profitul zilnic
    try
    {
        string limba;
        cout << "Selectati limba (Romana/Engleza): ";
        cin >> limba;
        int ok = 0;

        if (limba == "Engleza" || limba == "engleza" || limba == "ENGLEZA")
        {
            limba = "Engleza";
            ok = 1;
        }
        if (limba == "Romana" || limba == "ROMANA" || limba == "romana")
        {
            ok = 1;
        }
        if (ok == 0)
        {
            cout << "Limba nu este identificata";
            return 0;
        }

        string prefix = (limba == "Engleza" ? "engleza" : "romana");

        auto angajati = citesteCSV("angajati_" + prefix + ".csv");
        auto produse = citesteCSV("produse_" + prefix + ".csv");
        auto comenzi = citesteCSV("comenzi_" + prefix + ".csv");
        auto evenimente = citesteCSV("evenimente_" + prefix + ".csv");
        auto rapoarte = citesteCSV("rapoarte_" + prefix + ".csv");

        int optiune;

        do
        {
            cout << "\n--- " << (limba == "Engleza" ? "Main Menu" : "Meniu Principal") << " ---\n";
            cout << "1. " << (limba == "Engleza" ? "Employee Information" : "Informatii Angajati") << "\n";
            cout << "2. " << (limba == "Engleza" ? "Product Information" : "Informatii Produse") << "\n";
            cout << "3. " << (limba == "Engleza" ? "Order Information" : "Informatii Comenzi") << "\n";
            cout << "4. " << (limba == "Engleza" ? "Events Information" : "Informatii Evenimente") << "\n";
            cout << "5. " << (limba == "Engleza" ? "Show Financial Reports" : "Afiseaza Rapoarte Financiare") << "\n";
            cout << "0. " << (limba == "Engleza" ? "Exit" : "Iesire") << "\n";
            cout << "Selectati o optiune: ";
            cin >> optiune;

            switch (optiune)
            {
            case 1:
            {
                int optiuneAngajat;
                do
                {
                    cout << "\n--- " << (limba == "Engleza" ? "Employee Information Menu" : "Meniu Informatii Angajati") << " ---\n";
                    cout << "1. " << (limba == "Engleza" ? "Show Employees" : "Afiseaza Angajati") << "\n";
                    cout << "2. " << (limba == "Engleza" ? "Remove Employee" : "Sterge Angajat") << "\n";
                    cout << "3. " << (limba == "Engleza" ? "Add Employee" : "Adauga Angajat") << "\n";
                    cout << "4. " << (limba == "Engleza" ? "Change Employee's Schedule" : "Modificare Program Angajat") << "\n";
                    cout << "0. " << (limba == "Engleza" ? "Back to Main Menu" : "Inapoi la Meniul Principal") << "\n";
                    cout << "Selectati o optiune: ";
                    cin >> optiuneAngajat;

                    switch (optiuneAngajat)
                    {
                    case 1:
                    {
                        cout << "\n--- " << (limba == "Engleza" ? "Employees List" : "Lista Angajati") << " ---\n";
                        for (size_t i = 1; i < angajati.size(); ++i)
                        {
                            const auto &rand = angajati[i];
                            if (rand.size() < 5)
                            {
                                cout << "Date incomplete pentru angajatul de pe linia " << i + 1 << ".\n";
                                continue;
                            }

                            string numeComplet = rand[0] + " " + rand[1]; // Combinăm Nume și Prenume
                            Angajat *angajat = nullptr;
                            try
                            {
                                if (rand[2] == "Barista")
                                {
                                    angajat = new Barista(numeComplet, stoi(rand[3]), stoi(rand[4]));
                                }
                                else if (rand[2] == "Manager")
                                {
                                    angajat = new Manager(numeComplet, stoi(rand[3]), stoi(rand[4]));
                                }
                                else if (rand[2] == "Ospatar")
                                {
                                    angajat = new Ospatar(numeComplet, stoi(rand[3]), stoi(rand[4]));
                                }
                                else
                                {
                                    cout << "Functie necunoscuta pentru angajatul: " << numeComplet << ".\n";
                                    continue;
                                }
                                angajat->afiseazaInformatii();
                            }
                            catch (const exception &e) //exception
                            {
                                cout << "Eroare la procesarea datelor pentru angajatul: " << numeComplet << ".\n";
                            }
                            delete angajat;
                        }
                        break;
                    }

                    case 2:
                    {
                        cout << "\n--- " << (limba == "Engleza" ? "Remove Employee" : "Sterge Angajat") << " ---\n";
                        string nume;
                        cout << "Nume: ";
                        cin >> nume;
                        stergeAngajat("angajati_" + prefix + ".csv", nume);
                        cout << (limba == "Engleza" ? "Employee removed successfully." : "Angajat sters cu succes.") << "\n";
                        break;
                    }

                    case 3:
                    {
                        cout << "\n--- " << (limba == "Engleza" ? "Add Employee" : "Adauga Angajat") << " ---\n";
                        string nume, prenume, functie;
                        int oraInceput, oraSfarsit;

                        cout << (limba == "Engleza" ? "First Name: " : "Prenume: ");
                        cin >> prenume;
                        cout << (limba == "Engleza" ? "Last Name: " : "Nume: ");
                        cin >> nume;
                        cout << (limba == "Engleza" ? "Position (Barista/Manager/Waiter): " : "Functie (Barista/Manager/Ospatar): ");
                        cin >> functie;
                        cout << (limba == "Engleza" ? "Start Hour: " : "Ora inceput: ");
                        cin >> oraInceput;
                        cout << (limba == "Engleza" ? "End Hour: " : "Ora sfarsit: ");
                        cin >> oraSfarsit;

                        adaugaAngajat("angajati_" + prefix + ".csv", {nume, prenume, functie, to_string(oraInceput), to_string(oraSfarsit)});
                        cout << (limba == "Engleza" ? "Employee added successfully." : "Angajat adaugat cu succes.") << "\n";
                        break;
                    }

                    case 4:
                    {
                        cout << "\n--- " << (limba == "Engleza" ? "Manage Employee Schedule" : "Gestionare Program Angajat") << " ---\n";
                        string nume;
                        cout << (limba == "Engleza" ? "Enter full employee name: " : "Introdu numele si prenumele angajatului: ");
                        cin.ignore(); // Pentru a preveni probleme cu bufferul
                        getline(cin, nume);

                        // Cautăm angajatul în baza de date
                        bool found = false;
                        for (size_t i = 1; i < angajati.size(); ++i)
                        {
                            auto &rand = angajati[i];
                            if (rand.size() < 5)
                            {
                                continue;
                            }

                            string numeComplet = rand[0] + " " + rand[1];
                            if (numeComplet == nume)
                            {
                                found = true;

                                int oraInceput, oraSfarsit;
                                cout << (limba == "Engleza" ? "Enter new start hour: " : "Introdu noua ora de inceput: ");
                                cin >> oraInceput;
                                cout << (limba == "Engleza" ? "Enter new end hour: " : "Introdu noua ora de final: ");
                                cin >> oraSfarsit;

                                // Modificăm programul
                                rand[3] = to_string(oraInceput);
                                rand[4] = to_string(oraSfarsit);
                                cout << (limba == "Engleza" ? "Schedule updated successfully." : "Program actualizat cu succes.") << "\n";
                                break;
                            }
                        }

                        if (!found)
                        {
                            cout << (limba == "Engleza" ? "Employee not found." : "Angajatul nu a fost gasit.") << "\n";
                        }
                        else
                        {
                            // Rescriem fișierul CSV
                            ofstream fisier("angajati_" + prefix + ".csv");
                            if (fisier.is_open())
                            {
                                for (const auto &rand : angajati)
                                {
                                    for (size_t j = 0; j < rand.size(); ++j)
                                    {
                                        fisier << rand[j];
                                        if (j < rand.size() - 1)
                                        {
                                            fisier << ",";
                                        }
                                    }
                                    fisier << "\n";
                                }
                                fisier.close();
                                cout << (limba == "Engleza" ? "File updated successfully." : "Fisierul a fost actualizat cu succes.") << "\n";
                            }
                            else
                            {
                                cout << (limba == "Engleza" ? "Failed to update file." : "Nu s-a putut actualiza fisierul.") << "\n";
                            }
                        }
                        break;
                    }

                    case 0:
                        break;

                    default:
                        cout << (limba == "Engleza" ? "Invalid option." : "Optiune invalida.") << "\n";
                    }

                } while (optiuneAngajat != 0);
                break;
            }

            case 2:
            {
                int optiuneProdus;
                do
                {
                    cout << "\n--- " << (limba == "Engleza" ? "Product Information Menu" : "Meniu Informatii Produse") << " ---\n";
                    cout << "1. " << (limba == "Engleza" ? "Show Products" : "Afiseaza Produse") << "\n";
                    cout << "2. " << (limba == "Engleza" ? "Add Product" : "Adauga Produs") << "\n";
                    cout << "3. " << (limba == "Engleza" ? "Remove Product" : "Sterge Produs") << "\n";
                    cout << "0. " << (limba == "Engleza" ? "Back to Main Menu" : "Inapoi la Meniul Principal") << "\n";
                    cout << "Selectati o optiune: ";
                    cin >> optiuneProdus;

                    switch (optiuneProdus)
                    {
                    case 1:
                        cout << "\n--- " << (limba == "Engleza" ? "Products List" : "Lista Produse") << " ---\n";
                        for (size_t i = 1; i < produse.size(); ++i)
                        {
                            const auto &rand = produse[i];
                            cout << (limba == "Engleza" ? "Product: " : "Produs: ") << rand[0] << ", "
                                 << (limba == "Engleza" ? "Price: " : "Pret: ") << rand[1] << " RON\n";
                        }
                        break;

                    case 2:
                    {
                        cout << "\n--- " << (limba == "Engleza" ? "Add Product" : "Adauga Produs") << " ---\n";
                        string nume;
                        double pret;
                        cout << (limba == "Engleza" ? "Product Name: " : "Nume Produs: ");
                        cin.ignore();
                        getline(cin, nume);
                        cout << (limba == "Engleza" ? "Price: " : "Pret: ");
                        cin >> pret;

                        produse.push_back({nume, to_string(pret)});
                        scrieCSV("produse_" + prefix + ".csv", produse);
                        cout << (limba == "Engleza" ? "Product added successfully." : "Produs adaugat cu succes.") << "\n";
                        break;
                    }

                    case 3:
                    {
                        cout << "\n--- " << (limba == "Engleza" ? "Remove Product" : "Sterge Produs") << " ---\n";
                        string nume;
                        cout << (limba == "Engleza" ? "Product Name: " : "Nume Produs: ");
                        cin.ignore();
                        getline(cin, nume);

                        bool sters = false;
                        vector<vector<string>> produseActualizate;
                        for (const auto &rand : produse)
                        {
                            if (rand[0] == nume)
                            {
                                sters = true;
                            }
                            else
                            {
                                produseActualizate.push_back(rand);
                            }
                        }

                        if (sters)
                        {
                            scrieCSV("produse_" + prefix + ".csv", produseActualizate);
                            cout << (limba == "Engleza" ? "Product removed successfully." : "Produs sters cu succes.") << "\n";
                        }
                        else
                        {
                            cout << (limba == "Engleza" ? "Product not found." : "Produsul nu a fost gasit.") << "\n";
                        }
                        break;
                    }

                    case 0:
                        break;

                    default:
                        cout << (limba == "Engleza" ? "Invalid option." : "Optiune invalida.") << "\n";
                    }
                } while (optiuneProdus != 0);
                break;
            }

            case 3:
            {

                int optiuneComenzi;
                do
                {
                    cout << "\n--- " << (limba == "Engleza" ? "Order Information Menu" : "Meniu Informatii Comenzi") << " ---\n";
                    cout << "1. " << (limba == "Engleza" ? "Show Orders" : "Afiseaza Comenzi") << "\n";
                    cout << "2. " << (limba == "Engleza" ? "Add Order" : "Adauga Comanda") << "\n";
                    cout << "3. " << (limba == "Engleza" ? "Daily Profit" : "Profit Zilnic") << "\n";
                    cout << "0. " << (limba == "Engleza" ? "Back to Main Menu" : "Inapoi la Meniul Principal") << "\n";
                    cout << "Selectati o optiune: ";
                    cin >> optiuneComenzi;

                    switch (optiuneComenzi)
                    {
                    case 1:
                        cout << "\n--- " << (limba == "Engleza" ? "Orders List" : "Lista Comenzi") << " ---\n";
                        for (size_t i = 1; i < comenzi.size(); ++i)
                        {
                            const auto &rand = comenzi[i];
                            cout << (limba == "Engleza" ? "Order: " : "Comanda: ") << rand[0] << ", "
                                 << (limba == "Engleza" ? "Products: " : "Produse: ") << rand[1] << ", "
                                 << (limba == "Engleza" ? "Total: " : "Total: ") << rand[2] << " RON\n";
                        }
                        break;

                    case 2:
                    {
                        cout << "\n--- " << (limba == "Engleza" ? "Add Order" : "Adauga Comanda") << " ---\n";
                        string numeClient;
                        string produseComandate;
                        double total = 0.0;
                        vector<string> produseValid;

                        cout << (limba == "Engleza" ? "Customer Name: " : "Nume Client: ");
                        cin.ignore();
                        getline(cin, numeClient);

                        cout << (limba == "Engleza" ? "Enter products (comma-separated): " : "Introduceti produsele (separate prin virgula): ");
                        getline(cin, produseComandate);

                        stringstream ss(produseComandate);
                        string produs;

                        while (getline(ss, produs, ','))
                        {
                            produs.erase(remove_if(produs.begin(), produs.end(), ::isspace), produs.end());
                            bool gasit = false;
                            for (const auto &p : produse)
                            {
                                if (p[0] == produs)
                                {
                                    total += stod(p[1]);
                                    produseValid.push_back(produs);
                                    gasit = true;
                                    break;
                                }
                            }
                            if (!gasit)
                            {
                                cout << (limba == "Engleza" ? "Product not found: " : "Produsul nu a fost gasit: ") << produs << "\n";
                            }
                        }

                        int countClient = 0;
                        for (const auto &comanda : comenzi)
                        {
                            if (comanda[0] == numeClient)
                                countClient++;
                        }

                        if (countClient >= 3)
                        {
                            total *= 0.9; // Reducere de 10%
                            cout << (limba == "Engleza" ? "Thank you for being a loyal customer!" : "Multumim ca esti un client fidel!") << "\n";
                        }

                        if (!produseValid.empty())
                        {
                            string produseString = accumulate(next(produseValid.begin()), produseValid.end(), produseValid[0], [](string a, string b)
                                                              { return a + " " + b; });
                            comenzi.push_back({numeClient, produseString, to_string(total)});
                            scrieCSV("comenzi_" + prefix + ".csv", comenzi);
                            profitZilnic += total;
                            cout << (limba == "Engleza" ? "Order added successfully." : "Comanda adaugata cu succes.") << "\n";
                        }
                        else
                        {
                            cout << (limba == "Engleza" ? "No valid products. Order not added." : "Niciun produs valid. Comanda nu a fost adaugata.") << "\n";
                        }
                        break;
                    }

                    case 3:
                        cout << "\n--- " << (limba == "Engleza" ? "Daily Profit" : "Profit Zilnic") << " ---\n";
                        cout << (limba == "Engleza" ? "Current daily profit: " : "Profitul zilnic curent: ") << profitZilnic << " RON\n";
                        break;

                    case 0:
                        break;

                    default:
                        cout << (limba == "Engleza" ? "Invalid option." : "Optiune invalida.") << "\n";
                    }
                } while (optiuneComenzi != 0);
                break;
            }

            case 4:
            {
                int optiuneEveniment;
                do
                {
                    cout << "\n--- " << (limba == "Engleza" ? "Event Management Menu" : "Meniu Gestionare Evenimente") << " ---\n";
                    cout << "1. " << (limba == "Engleza" ? "Show Events" : "Afiseaza Evenimente") << "\n";
                    cout << "2. " << (limba == "Engleza" ? "Add Event" : "Adauga Eveniment") << "\n";
                    cout << "3. " << (limba == "Engleza" ? "Remove Event" : "Sterge Eveniment") << "\n";
                    cout << "0. " << (limba == "Engleza" ? "Back to Main Menu" : "Inapoi la Meniul Principal") << "\n";
                    cout << "Selectati o optiune: ";
                    cin >> optiuneEveniment;

                    switch (optiuneEveniment)
                    {
                    case 1:
                    {
                        cout << "\n--- " << (limba == "Engleza" ? "Events List" : "Lista Evenimente") << " ---\n";
                        for (size_t i = 1; i < evenimente.size(); ++i)
                        {
                            const auto &rand = evenimente[i];
                            Eveniment eveniment(rand[0], rand[1], stod(rand[2]));
                            eveniment.afiseazaEveniment();
                        }
                        break;
                    }

                    case 2:
                    {
                        cout << "\n--- " << (limba == "Engleza" ? "Add Event" : "Adauga Eveniment") << " ---\n";
                        string nume, data;
                        double cost;

                        cout << (limba == "Engleza" ? "Event Name: " : "Nume Eveniment: ");
                        cin.ignore();
                        getline(cin, nume);
                        cout << (limba == "Engleza" ? "Date (YYYY-MM-DD): " : "Data (YYYY-MM-DD): ");
                        getline(cin, data);
                        cout << (limba == "Engleza" ? "Cost: " : "Cost: ");
                        cin >> cost;

                        evenimente.push_back({nume, data, to_string(cost)});
                        scrieCSV("evenimente_" + prefix + ".csv", evenimente);
                        cout << (limba == "Engleza" ? "Event added successfully." : "Eveniment adaugat cu succes.") << "\n";
                        break;
                    }

                    case 3:
                    {
                        cout << "\n--- " << (limba == "Engleza" ? "Remove Event" : "Sterge Eveniment") << " ---\n";
                        string nume;
                        cout << (limba == "Engleza" ? "Event Name: " : "Nume Eveniment: ");
                        cin.ignore();
                        getline(cin, nume);

                        bool sters = false;
                        vector<vector<string>> evenimenteActualizate;
                        for (const auto &rand : evenimente)
                        {
                            if (rand[0] == nume)
                            {
                                sters = true;
                            }
                            else
                            {
                                evenimenteActualizate.push_back(rand);
                            }
                        }

                        if (sters)
                        {
                            scrieCSV("evenimente_" + prefix + ".csv", evenimenteActualizate);
                            cout << (limba == "Engleza" ? "Event removed successfully." : "Eveniment sters cu succes.") << "\n";
                        }
                        else
                        {
                            cout << (limba == "Engleza" ? "Event not found." : "Evenimentul nu a fost gasit.") << "\n";
                        }
                        break;
                    }

                    case 0:
                        break;

                    default:
                        cout << (limba == "Engleza" ? "Invalid option." : "Optiune invalida.") << "\n";
                    }

                } while (optiuneEveniment != 0);
                break;
            }

            case 5:

                cout << "\n--- " << (limba == "Engleza" ? "Financial Reports" : "Rapoarte Financiare") << " ---\n";

                // Calcularea veniturilor și costurilor
                double venituri = profitZilnic;
                double costuri = 50.0; // Costurile fixe
                double profitTotal = venituri - costuri;

                // Afișarea informațiilor în terminal
                cout << (limba == "Engleza" ? "Daily Revenues: " : "Venituri zilnice: ") << venituri << " RON\n";
                cout << (limba == "Engleza" ? "Daily Costs: " : "Costuri zilnice: ") << costuri << " RON\n";
                cout << (limba == "Engleza" ? "Total Profit: " : "Profit total: ") << profitTotal << " RON\n";

                // Adăugarea acestor date în fișierul CSV

                rapoarte.push_back({to_string(venituri), to_string(costuri), to_string(profitTotal)});

                string fisierRaport = "rapoarte_" + prefix + ".csv";
                scrieCSV(fisierRaport, rapoarte);

                break;
            }

        } while (optiune != 0);
    }
    catch (const exception &e)
    {
        cerr << "Eroare: " << e.what() << "\n";
    } // exceptions

    return 0;
}