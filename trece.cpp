#include <iostream>
#include <iomanip>
#include <limits>
#include <new>
using namespace std;

float** crtMat(int f, int c) {
    float** m = nullptr;
    try {
        m = new float*[f];
        for (int i = 0; i < f; i++) {
            m[i] = new float[c];
            for (int j = 0; j < c; j++) m[i][j] = 0;
        }
    } catch (bad_alloc&) {
        return nullptr;
    }
    return m;
}

void delMat(float** m, int f) {
    for (int i = 0; i < f; i++) delete[] m[i];
    delete[] m;
}

bool addSem(float*** pm, int f, int& c) {
    int nc = c + 1;
    float** nm = nullptr;
    try {
        nm = new float*[f];
        for (int i = 0; i < f; i++) {
            nm[i] = new float[nc];
            for (int j = 0; j < c; j++) nm[i][j] = (*pm)[i][j];
            nm[i][c] = 0;
        }
    } catch (bad_alloc&) {
        if (nm) {
            for (int i = 0; i < f; i++) if (nm[i]) delete[] nm[i];
            delete[] nm;
        }
        return false;
    }
    for (int i = 0; i < f; i++) delete[] (*pm)[i];
    delete[] (*pm);
    *pm = nm;
    c = nc;
    return true;
}

float* crtVec(int t, float v = 0) {
    float* x = nullptr;
    try {
        x = new float[t];
        for (int i = 0; i < t; i++) x[i] = v;
    } catch (bad_alloc&) {
        return nullptr;
    }
    return x;
}

void shwMat(float** m, int f, int c) {
    cout << fixed << setprecision(2);
    cout << "\nPlan (" << f << "x" << c << ")\n";
    cout << "P\\S ";
    for (int j = 0; j < c; j++) cout << setw(8) << j;
    cout << "\n";
    for (int i = 0; i < f; i++) {
        cout << "P" << i << " ";
        for (int j = 0; j < c; j++) cout << setw(8) << m[i][j];
        cout << "\n";
    }
}

bool valInt(int& x, int a, int b) {
    if (!(cin >> x)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    if (x < a || x > b) return false;
    return true;
}

bool valFlt(float& x) {
    if (!(cin >> x)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    return true;
}

void getCon(float** pl, int pr, int sm, int s, const int rq[5][4], float* out) {
    for (int k = 0; k < 4; k++) out[k] = 0;
    for (int i = 0; i < pr; i++) {
        float q = pl[i][s];
        for (int k = 0; k < 4; k++) out[k] += q * rq[i][k];
    }
}

void getCogs(float** pl, int pr, int sm, int s, float* cu, float* mp, const float pm[4], const int rq[5][4]) {
    float pt = 0, ct = 0;
    for (int i = 0; i < pr; i++) {
        float q = pl[i][s];
        pt += q;
        ct += q * cu[i];
    }
    float cs[4];
    getCon(pl, pr, sm, s, rq, cs);
    float vi = 0, vf = 0, cp = 0;
    for (int k = 0; k < 4; k++) {
        vi += mp[k] * pm[k];
        float r = mp[k] - cs[k];
        if (r < 0) {
            cp += (-r) * pm[k];
            r = 0;
        }
        vf += r * pm[k];
    }
    float cg = vi + cp - vf;
    cout << fixed << setprecision(2);
    cout << "\n--- COGS (Semana " << s << ") ---\n";
    cout << "Prod total: " << pt << "\n";
    cout << "Costo prod: $" << ct << "\n";
    cout << "COGS: $" << cg << "\n";
    cout << "Inv final MP: $" << vf << "\n";
    cout << "Consumo MP:\n";
    for (int k = 0; k < 4; k++) cout << "MP" << (k+1) << ": " << cs[k] << " u, $" << pm[k] << "\n";
}

int main() {
    const int PR = 5;
    int pr = PR, sm = 4;
    float** pl = crtMat(pr, sm);
    if (!pl) return 1;
    float* cu = crtVec(pr, 0);
    float* mp = crtVec(4, 0);
    const float pm[4] = {2.5f, 1.75f, 0.95f, 4.2f};
    const int rq[5][4] = {
        {2,1,0,0},
        {1,2,1,0},
        {0,1,2,1},
        {3,0,1,1},
        {1,1,1,2}
    };
    cu[0]=5; cu[1]=6.5; cu[2]=3.75; cu[3]=7.2; cu[4]=4.4;
    mp[0]=1000; mp[1]=800; mp[2]=500; mp[3]=300;
    for (int i=0;i<pr;i++) for (int j=0;j<sm;j++) pl[i][j]=(i+1)*(j+1)*10;

    bool end=false;
    while(!end){
        cout<<"\n--- Sistema COGS ---\n";
        cout<<"1.Ver Plan\n2.Agregar Semana\n3.Modificar Prod\n4.Calcular COGS\n5.Consumo MP\n6.Inventario MP\n7.Salir\nOpcion:";
        int op;
        if(!valInt(op,1,7)){cout<<"Invalido\n";continue;}
        if(op==1) shwMat(pl,pr,sm);
        else if(op==2){
            cout<<"Agregando semana "<<sm<<"...\n";
            if(addSem(&pl,pr,sm)) cout<<"OK "<<pr<<"x"<<sm<<"\n"; else cout<<"Error\n";
        }
        else if(op==3){
            int p,s; float q;
            cout<<"Prod(0-"<<pr-1<<"):"; if(!valInt(p,0,pr-1)){cout<<"Inv\n";continue;}
            cout<<"Sem(0-"<<sm-1<<"):"; if(!valInt(s,0,sm-1)){cout<<"Inv\n";continue;}
            cout<<"Cant:"; if(!valFlt(q)||q<0){cout<<"Inv\n";continue;}
            pl[p][s]=q;
            cout<<"Act: P"<<p<<" S"<<s<<"="<<q<<"\n";
        }
        else if(op==4){
            int s;
            cout<<"Semana(0-"<<sm-1<<"):"; if(!valInt(s,0,sm-1)){cout<<"Inv\n";continue;}
            getCogs(pl,pr,sm,s,cu,mp,pm,rq);
        }
        else if(op==5){
            int s; cout<<"Semana(0-"<<sm-1<<"):"; if(!valInt(s,0,sm-1)){cout<<"Inv\n";continue;}
            float cs[4]; getCon(pl,pr,sm,s,rq,cs);
            cout<<"\nConsumo MP (Sem "<<s<<")\n";
            for(int k=0;k<4;k++) cout<<"MP"<<(k+1)<<": "<<cs[k]<<"\n";
        }
        else if(op==6){
            cout<<"\nInv MP:\n";
            for(int k=0;k<4;k++) cout<<"MP"<<(k+1)<<": "<<mp[k]<<" u, $"<<pm[k]<<"\n";
            cout<<"Costos Prod:\n";
            for(int i=0;i<pr;i++) cout<<"P"<<i<<": $"<<cu[i]<<"\n";
        }
        else if(op==7){
            cout<<"Liberando...\n";
            end=true;
        }
    }
    delMat(pl,pr);
    delete[] cu;
    delete[] mp;
    cout<<"Fin\n";
    return 0;
}

