/*
* Este programa toma un numero muy grande y a partir de sus digitos crea una imagen bmp
* No importa que el archivo contenga caracteres no numericos, pues se encarga de eliminarlos.
* El programa tambien determina el tamaño de la imagen dependiendo de el numero de datos que se quieran graficar
*
* Autores: Leiver Andres Campeón
*          Jorge Mario Gil
* Fecha: 30 Nov 2015
* Version: V 3.1
*
*/
#include <bits/stdc++.h>
#define LIMIT 10000000
using namespace std;

int H = 1;
int W = 1;
vector <int> primes;

int str_to_int(string s) {
  istringstream buffer(s);
  int value;
  buffer >> value;
  return value;
}

bool is_num(char a) {
    return (a > '0' && a < '9');
}

void primes_generator(int n) {//n max number until i'll calculate
//in order to factorize the number (n) not gonna need a prime number greater than sqrt(n)
  int sqrtN = int(sqrt(n)) + 1;
  vector <bool> sieve(sqrtN, false);
  sieve[0] = sieve[1] = true;
  for (int i = 2; i < sqrtN; ++i) {
    if (!sieve[i]) primes.push_back(i);
    for (int j = i+i; j < sqrtN; j += i) {
      sieve[j] = true;
    }
  }
}

void prime_factorize( int n ) {
    vector <int> factors;
    int aux_n = n;
    for(int i = 0; i < primes.size(); ++i) {
        if(aux_n % primes[i] == 0) {
            while(aux_n % primes[i] == 0) {
                aux_n /= primes[i];
                factors.push_back(primes[i]);
            }
        }
    }
    if( aux_n > 1 ) {
        // aux_n is greater than 1, so we are sure that this aux_n is a prime
        factors.push_back(aux_n); // added n (the prime) in the list
    }

    ////////////////// test ////////////////////////////////////
    cout << "Factores del numero: " << n <<" :\n";
    for (int i = 0; i < factors.size(); ++i) {
      cout<<factors[i]<<" ";
    }
    cout<<endl;
    ////////////////// test ////////////////////////////////////

    ///////////////////// derteminate Height and Width //////////////////////
    int dif  = 0;
    for (int i = 0; i < factors.size(); ++i){
      if (abs((H*factors[i]) - W) <  abs(H - (W*factors[i])))
        H *= factors[i];
      else
        W *= factors[i];
    }
    //set a limit of pixels, to be able to show it
    if (H > 1200) {
      H = 1200;
    }
    if (W > 1200) {
      W = 1200;
    }

    //and this is because We want a semi-square image. dimension diference no greater than 400
    if ((H - W) < -400) {
      H += 400;
      W -= 400;
    } else if ((H - W) > 400) {
      W += 400;
      H -= 400;
    }
    ////////////////// test ////////////////////////////////////
    // cout << "Las dimensiones de las matrices: (H, W) " << H << " " << W << endl;
   ////////////////// test ////////////////////////////////////
}

int main() {
  string s;
  string rest = "";
  int aux_num = 0, count = 0, elem_per_color = 0;
  int counter = 0;
  vector <int> number;//here I have rgb, I don't need them physically
  // int red[H][W],
  //     blue[H][W],
  //     green[H][W];

  ifstream fin("sqrt5_web.txt");
  ofstream fout("output_sqrt5.txt");
  while (!fin.eof() && counter < LIMIT) {
    string s;
    fin >> s;

    for (int i = 0; i < s.size(); ++i) {
      if (is_num(s[i])) {
          fout << s[i];
          counter++;
      }
    }


  }
  cout << "Numeros procesados: " << counter << endl;
  fout.close();
  fin.close();

  ifstream finput("output_sqrt5.txt");

  ///////////// dividing Number each 3 digits ////////////////////////////////////////
  string aux_str = "";
  int total_nums = 0;//total of numebers processed
  bool writeEn = false;//is true when It should add a numeber to the vector, false otherwise
  while(!finput.eof()) {

    finput >> s;// get each line
    s = rest + s;
    // cout<<"before eliminate "<<s.size()<<endl;
    //s.erase(std::remove(s.begin(), s.end(), '\n'), s.end());//eliminar todos los \n
    // cout<<"after eliminate "<<s.size()<<endl;
    total_nums += s.size() - rest.size();
    for (int i = 0; i < s.size(); ++i){
      if ((s.size() - i) >= 3) {
        if (s[i] != '\n') { //just in case that a \n has passed
          aux_str += s[i];
          count++;
        }

        if(count == 3){
          aux_num = str_to_int(aux_str);
          aux_str = "";
          count = 0;
          writeEn = true;
        }
      } else {
        rest = s.substr(i);
        break;
      }

      if (writeEn){
        if (aux_num > 255) {
          number.push_back( rand()%250 );
        } else {
          number.push_back(aux_num);
        }
        writeEn = false;
      }
    }
  }

  ////////////////// test ////////////////////////////////////
  cout<<"Tamaño del arreglo: "<<number.size()<<endl;
  // cout<<"numeros procesados: "<<total_nums<<endl;
  ////////////////// test ////////////////////////////////////

  finput.close();
  ///////////// saving number in the matrices ///////////////////////////////////////////

  elem_per_color = number.size();
  // cout<< "each matrix should have: " << elem_per_color << " numbers."<<endl;

  primes_generator(elem_per_color/3);
  prime_factorize(elem_per_color/3);

  unsigned char *img = NULL;
  img = (unsigned char *)malloc(3*W*H);
  int x = 0, y = 0;
  for (int i = 0; i < W; ++i) {
    for (int j = 0; j < H; ++j) {
      x=i; y=(H-1)-j;
      img[(x+y*W)*3+2] = (unsigned char)(number[(x+y*W)*3+2]);
      img[(x+y*W)*3+1] = (unsigned char)(number[(x+y*W)*3+1]);
      img[(x+y*W)*3+0] = (unsigned char)(number[(x+y*W)*3+0]);
    }
  }

  cout << "Las dimensiones de las matrices: (H, W) " << H << " " << W << endl;

  int filesize = 54 + ( H*W*3);
  unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
  unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
  unsigned char bmppad[3] = {0,0,0};

  bmpfileheader[ 2] = (unsigned char)(filesize    );
  bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
  bmpfileheader[ 4] = (unsigned char)(filesize>>16);
  bmpfileheader[ 5] = (unsigned char)(filesize>>24);

  bmpinfoheader[ 4] = (unsigned char)(       W    );
  bmpinfoheader[ 5] = (unsigned char)(       W>> 8);
  bmpinfoheader[ 6] = (unsigned char)(       W>>16);
  bmpinfoheader[ 7] = (unsigned char)(       W>>24);
  bmpinfoheader[ 8] = (unsigned char)(       H    );
  bmpinfoheader[ 9] = (unsigned char)(       H>> 8);
  bmpinfoheader[10] = (unsigned char)(       H>>16);
  bmpinfoheader[11] = (unsigned char)(       H>>24);

  FILE *f;
  // ofstream foutput("result.bmp");
  // for (int i = 0; i < 14; ++i)
  //   foutput << bmpfileheader[i];
  // for (int i = 0; i < 40; i++)
  //   foutput << bmpinfoheader;
  // for (int i = 0; i < H; ++i) {
  //   for (int j = (W*(H-i-1)*3); j < ((W*(H-i-1)*3) + 3); ++j) {
  //     foutput << img[j];
  //   }
  //   for(int k = 0; k < (4-(W*3)%4)%4; ++k) {
  //     foutput << bmppad[k];
  //   }
  // }
  //   // foutput << (unsigned char)number[i];
  // foutput.close();


  f = fopen("sqrt5.bmp","wb");

  fwrite(bmpfileheader,1,14,f);
  fwrite(bmpinfoheader,1,40,f);

  for(int i = 0; i < H; ++i) {
      fwrite(img+(W*(H-i-1)*3),3,W,f);
      fwrite(bmppad,1,(4-(W*3)%4)%4,f);
  }
  fclose(f);
  return 0;
}
