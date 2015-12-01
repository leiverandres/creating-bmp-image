/*
* Este programa toma un numero muy grande y a partir de sus digitos crea una imagen bmp
* No importa que el archivo contenga caracteres no numericos, pues se encarga de eliminarlos.
* El programa tambien determina el tamaño de la imagen dependiendo de el numero de datos que se quieran graficar
*
* Autores: Leiver Andres Campeón.
*          Jorge Mario Gil.
* Fecha: 30 Nov 2015
* Version: V 3.3
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
  return (a >= '0' && a <= '9');
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

  ///////////////// derteminate Height and Width /////////////
  int dif  = 0;
  for (int i = 0; i < factors.size(); ++i){
    if (abs((H*factors[i]) - W) <  abs(H - (W*factors[i])))
      H *= factors[i];
    else
      W *= factors[i];
  }
  //set a limit of pixels, to be able to show it on a standar screen
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
  int aux_num = 0,
      elem_per_color = 0,
      counter = 0,
      count = 0;

  vector <int> number;//here I have rgb, I don't need them physically
  // int red[H][W],
  //     blue[H][W],
  //     green[H][W];

  ////////////// Filter. take no numeric chars out the file ////////////////////
  ifstream fin("pi_web.txt");
  ofstream fout("output_pi.txt");//in this file I'll have the number after filter
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

  //// after this I gonna have the file without no numeric chars///////////////

  ///////////// dividing Number each 3 digits /////////////////////////////////
  ifstream finput("output_pi.txt");
  string aux_str = "";
  bool writeEn = false;//is true when It should add a numeber to the vector, false otherwise
  while(!finput.eof()) {
    finput >> s;// get each line
    s = rest + s;
    for (int i = 0; i < s.size(); ++i) {
      if ( (s.size() - i) >= 3 ) {
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
        if (aux_num > 255) {//if the value is grater I just pick up a rand number
          number.push_back( rand()%250 );
        } else {
          number.push_back(aux_num);
        }
        writeEn = false;
      }
    }
  }
  finput.close();
  ////////////////// test ////////////////////////////////////
  cout<<"Tamaño del arreglo: "<<number.size()<<endl;
  ////////////////// test ////////////////////////////////////

  ///////////// saving number in the matrices ///////////////////////////////////////////

  elem_per_color = number.size()/3;
  // cout<< "each matrix should have: " << elem_per_color << " numbers."<<endl;

  primes_generator(elem_per_color);
  prime_factorize(elem_per_color);

  unsigned char *img = NULL;//final matrix to write in the file.
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
  unsigned char bmpfileheader[14] = {'B','M',//Id format
                                      0,0,0,0, //SizeFile
                                      0,0,//reserved
                                      0,0,//reserved
                                      54,0,0,0//start data(offset)
                                    };
  unsigned char bmpinfoheader[40] = {40,0,0,0,//headerSize
                                     0,0,0,0,//Width
                                     0,0,0,0,//Height
                                     1,0,//bitPlanes must be 1
                                     24,0,//bits per pixel
                                     0,0,0,0,//Compression type
                                     0,0,0,0,//Image Size, 0 for uncompressed Im
                                     0x13,0x0B,0,0,//X resolution in pixels per meter
                                     0x13,0x0B,0,0,//Y resolution in pixels per meter
                                     0,0,0,0,//Number Color Map
                                     0,0,0,0,//Number of significant colors
                                    };

  unsigned char bmppad[3] = {0,0,0};//padding, zero by default

  //corrimiento de bytes:la info de filesize, W y H ocupan 4 bytes en el archivo bmp
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

  ///// finally writing results ///////////////////////////////
  FILE *f;
  f = fopen("pi.bmp","wb");

  fwrite(bmpfileheader,1,14,f);
  fwrite(bmpinfoheader,1,40,f);

  for(int i = 0; i < H; ++i) {
      fwrite(img+(W*(H-i-1)*3),3,W,f);
      fwrite(bmppad,1,(4-(W*3)%4)%4,f);
  }
  fclose(f);
  //////////////////////////////////////////////////////////////
  return 0;
}
