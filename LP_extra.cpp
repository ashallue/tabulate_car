// threaded version of cars5
void LargePreproduct::cars5_threaded(string cars_file, long thread, long num_threads){
  //setup file
  ofstream output;
  output.open(cars_file);

  // primes out of the primes index, their indices
  long p1, p2, p3, q;
  long i1, i2, i3, i4;
  // lower bounds are given in terms of index, uppers in terms of values
  long lower_index;
  long upper1, upper2, upper3, upper4;

  // keep running computation of P and lcm_p|P p-1
  bigint P1, P2, P3, P4;
  bigint L1, L2, L3, L4;
  long g;

  vector<long> rs;

  // nested for loops
  // compute first upper bound as B^{1/5}
  upper1 = find_upper(B, 1, 5);
  cout << "upper1 = " << upper1 << "\n";

  // start p1 at the prime corresponding to thread number
  i1 = thread;
  p1 = primes[i1];
  P1 = p1;
  do{
    // compute L1
    L1 = p1 - 1;

    // start off p2 at the next prime
    i2 = i1 + 1;

    // also need to compute the corresponding upper bound: (B/p1)^{1/4}
    upper2 = find_upper(B, p1, 4);
    cout << "then lower_index = " << lower_index << " and upper2 = " << upper2 << "\n";

    // finding the start index for p2
    p2 = primes[i2];
    // check admissability, bump ahead until found
    while( gcd( p2 - 1, P1) != 1){
      i2++;
      p2 = primes[i2];
    }
    P2 = P1 * p2;
    
    cout << "first P2 is " << P2 << "\n";
    if(P2 == 5 * 13) cout << "P2 takes value 5 * 13\n";

    do{
      if(p1 == 5) cout << "p2 = " << p2 << "\n";

      //update L2
      L2 = L1 * (p2 - 1);
      g = gcd(L1, p2 - 1);
      L2 = L2 / g;

      // if p1 * p2 * p2 > X, take i3 to be i2 + 1.  Otherwise bound is X / p1p2
      if(p1 * p2 * p2 > X){
        lower_index = i2 + 1;
      }else{
        lower_index = find_index_lower( X / P2 );
      }
      i3 = lower_index;

      // upper bound is (B/p1p2)^{1/3}
      upper3 = find_upper(B, P2, 3);
      
      // find start index for p3
      p3 = primes[i3];
      // check admissability
      while( gcd(p3 - 1, P2) != 1){
        i3++;
        p3 = primes[i3];
      }
      P3 = P2 * p3;

      do{

        // update L3
        L3 = L2 * (p3 - 1);
        g = gcd(L2, p3 - 1);
        L3 = L3 / g;

        // lower bound for q is just the previous prime, upper is (B/p1p2p3)^{1/2}
        upper4 = find_upper(B, P3, 2);
        
        // finding the start index and prime for q
        i4 = i3 + 1;
        q = primes[i4];
        // check admissability, bump ahead until found
        while( gcd( q - 1, P3 ) != 1 ){
          i4++;
          q = primes[i4];
        }
        P4 = P3 * q;

        do{
          // update L3
          L4 = L3 * (q - 1);
          g = gcd(L3, q - 1);
          L4 = L4 / g;

          if(p1 * p2 * p3 * q == 44757505){
          cout << "Inner loop with p1 = " << p1 << " p2 = " << p2 << " p3 = " << p3 << " q = " << q << "\n";
          }
          // complicated inner loop work that finds r's that make carmichaels
          // clears rs vector and refills it
          inner_loop_work(P4, q, L4, rs);

          // write to file
          for(long i = 0; i < rs.size(); i++){
            // note this next line might attempt to print a bigint and faile
            output << P4 * rs[i] << " ";
            output << p1 << " " << p2 << " " << p3 << " " << q << " " << rs[i] << "\n";
          }

          // find next q that makes P2 * q admissable
          do{
            i4++;
            q = primes[i4];
          }while( gcd( q - 1, P3 ) != 1 );
          P4 = P3 * q;

        } while(q < upper4); // end of do q
 
        // find next p3 that makes p1*p2*p3 admissable
        do{
          i3++;
          p3 = primes[i3];
        }while( gcd( p3 - 1, P2 ) != 1 );
        P3 = P2 * p3;

      }while(p3 < upper3);  // end of do p3

      // find next p2 that makes p1*p2 admissable
      do{
        i2++;
        p2 = primes[i2];
      }while( gcd( p2 - 1, P1 ) != 1 );
      P2 = P1 * p2;

    }while(p2 < upper2);  // end of do p2

    // next prime p1
    i1 += num_threads;
    p1 = primes[i1];
    P1 = p1;
  }while(p1 < upper1);  // end of do p1

  output.close();
}

// threaded version of cars6
void LargePreproduct::cars6_threaded(string cars_file, long thread, long num_threads){
  //setup file
  ofstream output;
  output.open(cars_file);

  // primes out of the primes index, their indices
  long p1, p2, p3, p4, q;
  long i1, i2, i3, i4, i5;
  // lower bounds are given in terms of index, uppers in terms of values
  long lower_index;
  long upper1, upper2, upper3, upper4, upper5;

  // keep running computation of P and lcm_p|P p-1
  bigint P1, P2, P3, P4, P5;
  bigint L1, L2, L3, L4, L5;
  long g;

  vector<long> rs;

  // nested for loops
  // compute first upper bound as B^{1/6}
  upper1 = find_upper(B, 1, 6);
  cout << "upper1 = " << upper1 << "\n";

  // start p1 at the prime corresponding to thread number
  i1 = thread;
  p1 = primes[i1];
  P1 = p1;
  do{

    // compute L1
    L1 = p1 - 1;

    // take p2 to be the next prime after p1, though I need to check admissability
    i2 = i1 + 1;

    // also need to compute the corresponding upper bound: (B/p1)^{1/5}
    upper2 = find_upper(B, p1, 5);
    //cout << "then lower_index = " << lower_index << " and upper2 = " << upper2 << "\n";

    // finding the start index for p2
    p2 = primes[i2];
    // check admissability, bump ahead until found
    while( gcd( p2 - 1, P1) != 1){
      i2++;
      p2 = primes[i2];
    }
    P2 = P1 * p2;

    do{

      //update L2
      L2 = L1 * (p2 - 1);
      g = gcd(L1, p2 - 1);
      L2 = L2 / g;

      // take p3 to be the next prime after p2
      i3 = i2 + 1;

      // upper bound is (B/p1p2)^{1/4}
      upper3 = find_upper(B, P2, 4);
      
      // find start index for p3
      p3 = primes[i3];
      // check admissability
      while( gcd(p3 - 1, P2) != 1){
        i3++;
        p3 = primes[i3];
      }
      P3 = P2 * p3;

      do{

        // update L3
        L3 = L2 * (p3 - 1);
        g = gcd(L2, p3 - 1);
        L3 = L3 / g;

        // if p1 * p2 * p3^2 > X take i4 = i3 + 1.  Otherwise bound is (X / p1p2p3)
        if(P3 * p3 > X){
          lower_index = i3 + 1;
        }else{
          lower_index = find_index_lower( X / P3 );
        }
        i4 = lower_index;

        // upper bound is (B/p1p2p3)^{1/3}
        upper4 = find_upper(B, P3, 3);

        // find start index for p4
        p4 = primes[i4];
        // check admissability
        while( gcd(p4 - 1, P3) != 1){
          i4++;
          p4 = primes[i4];
        }
        P4 = P3 * p4;

        do{
          // update L4
          L4 = L3 * (p4 - 1);
          g = gcd(L3, p4 - 1);
          L4 = L4 / g;

          // lower bound for q is just the previous prime, upper is (B/p1p2p3p4)^{1/2}
          upper5 = find_upper(B, P4, 2);
        
          // finding the start index and prime for q
          i5 = i4 + 1;
          q = primes[i5];
          // check admissability, bump ahead until found
          while( gcd( q - 1, P4 ) != 1 ){
            i5++;
            q = primes[i5];
          }
          P5 = P4 * q;

          do{
            // update L5
            L5 = L4 * (q - 1);
            g = gcd(L4, q - 1);
            L5 = L5 / g;

            //cout << "Inner loop with p1 = " << p1 << " p2 = " << p2 << " p3 = " << p3 << " p4 = " << p4 << " q = " << q << "\n";

            // complicated inner loop work that finds r's that make carmichaels
            // clears rs vector and refills it
            inner_loop_work(P5, q, L5, rs);

            // write to file
            for(long i = 0; i < rs.size(); i++){
              // note this next line might attempt to print a bigint and faile
              output << P5 * rs[i] << " ";
              output << p1 << " " << p2 << " " << p3 << " " << p4 << " " << q << " " << rs[i] << "\n";
            }

            // find next q that makes P2 * q admissable
            do{
              i5++;
              q = primes[i5];
            }while( gcd( q - 1, P4 ) != 1 );
            P5 = P4 * q;

          } while(q < upper5); // end of do q
          
          // find next p4
          do{
            i4++;
            p4 = primes[i4];
          }while( gcd( p4 - 1, P3 ) != 1);
          P4 = P3 * p4;

        }while(p4 < upper4);  // end of do p4
 
        // find next p3 that makes p1*p2*p3 admissable
        do{
          i3++;
          p3 = primes[i3];
        }while( gcd( p3 - 1, P2 ) != 1 );
        P3 = P2 * p3;
 
      }while(p3 < upper3);  // end of do p3

      // find next p2 that makes p1*p2 admissable
      do{
        i2++;
        p2 = primes[i2];
      }while( gcd( p2 - 1, P1 ) != 1 );
      P2 = P1 * p2;

    }while(p2 < upper2);  // end of do p2

    // next prime p1
    i1 += num_threads;
    p1 = primes[i1];
    P1 = p1;
  }while(p1 < upper1);  // end of do p1

  output.close();
}

// threaded version of cars7
void LargePreproduct::cars7_threaded(string cars_file, long thread, long num_threads){
  //setup file
  ofstream output;
  output.open(cars_file);

  // primes out of the primes index, their indices
  long p1, p2, p3, p4, p5, q;
  long i1, i2, i3, i4, i5, i6;
  // lower bounds are given in terms of index, uppers in terms of values
  long lower_index;
  long upper1, upper2, upper3, upper4, upper5, upper6;

  // keep running computation of P and lcm_p|P p-1
  bigint P1, P2, P3, P4, P5, P6;
  bigint L1, L2, L3, L4, L5, L6;
  long g;

  vector<long> rs;

  // nested for loops
  // compute first upper bound as B^{1/7}
  upper1 = find_upper(B, 1, 7);
  cout << "upper1 = " << upper1 << "\n";

  // start p1 at the prime corresponding to thread number
  i1 = thread;
  p1 = primes[i1];
  P1 = p1;
  do{

    // compute L1
    L1 = p1 - 1;

    // take p2 to be next prime after p1
    i2 = i1 + 1;

    // also need to compute the corresponding upper bound: (B/p1)^{1/6}
    upper2 = find_upper(B, p1, 6);
    //cout << "then lower_index = " << lower_index << " and upper2 = " << upper2 << "\n";

    // finding the start index for p2
    p2 = primes[i2];
    // check admissability, bump ahead until found
    while( gcd( p2 - 1, P1) != 1){
      i2++;
      p2 = primes[i2];
    }
    P2 = P1 * p2;

    do{

      //update L2
      L2 = L1 * (p2 - 1);
      g = gcd(L1, p2 - 1);
      L2 = L2 / g;

      // take p3 to be next prime after p2 
      i3 = i2 + 1;

      // upper bound is (B/p1p2)^{1/5}
      upper3 = find_upper(B, P2, 5);
      
      // find start index for p3
      p3 = primes[i3];
      // check admissability
      while( gcd(p3 - 1, P2) != 1){
        i3++;
        p3 = primes[i3];
      }
      P3 = P2 * p3;

      do{

        // update L3
        L3 = L2 * (p3 - 1);
        g = gcd(L2, p3 - 1);
        L3 = L3 / g;

        // take p4 to be next prime after p3
        i4 = i3 + 1;

        // upper bound is (B/p1p2p3)^{1/4}
        upper4 = find_upper(B, P3, 4);

        // find start index for p4
        p4 = primes[i4];
        // check admissability
        while( gcd(p4 - 1, P3) != 1){
          i4++;
          p4 = primes[i4];
        }
        P4 = P3 * p4;

        do{
          // update L4
          L4 = L3 * (p4 - 1);
          g = gcd(L3, p4 - 1);
          L4 = L4 / g;

          // if p1 * p2 * p3 * p4^2 > X take i5 = i4 + 1.  Otherwise X / p1p2p3p4
          if(P4 * p4 > X){
            lower_index = i4 + 1;
          }else{
            lower_index = find_index_lower( X / P4 );
          }
          i5 = lower_index;

          // upper bound is (B/p1p2p3p4)^{1/3}
          upper5 = find_upper(B, P4, 3);

          // find start index for p5, discarding choices not admissable
          p5 = primes[i5];
          while( gcd(p5 - 1, P4) != 1 ){
            i5++;
            p5 = primes[i5];
          }
          P5 = P4 * p5;

          do{
            // update L5
            L5 = L4 * (p5 - 1);
            g = gcd(L4, p5 - 1);
            L5 = L5 / g;

            // lower bound for q is just the previous prime, upper is (B/p1p2p3p4p5)^{1/2}
            upper6 = find_upper(B, P5, 2);
        
            // finding the start index and prime for q
            i6 = i5 + 1;
            q = primes[i6];
            // check admissability, bump ahead until found
            while( gcd( q - 1, P5 ) != 1 ){
              i6++;
              q = primes[i6];
            }
            P6 = P5 * q;

            do{
              // update L5
              L6 = L5 * (q - 1);
              g = gcd(L5, q - 1);
              L6 = L6 / g;

              //cout << "Inner loop with p1 = " << p1 << " p2 = " << p2 << " p3 = " << p3 << " p4 = " << p4 << " p5 = " << p5 << " q = " << q << "\n";

              // complicated inner loop work that finds r's that make carmichaels
              // clears rs vector and refills it
              inner_loop_work(P6, q, L6, rs);

              // write to file
              for(long i = 0; i < rs.size(); i++){
                // note this next line might attempt to print a bigint and faile
                output << P6 * rs[i] << " ";
                output << p1 << " " << p2 << " " << p3 << " " << p4 << " " << p5 << " " << q << " " << rs[i] << "\n";
              }

              // find next q that makes P2 * q admissable
              do{
                i6++;
                q = primes[i6];
              }while( gcd( q - 1, P5 ) != 1 );
              P6 = P5 * q;

            } while(q < upper6); // end of do q

            // find next p5
            do{
              i5++;
              p5 = primes[i5];
            }while( gcd( p5 - 1, P4 ) != 1 );
            P5 = P4 * p5;

          }while(p5 < upper5); // end of do p5
          
          // find next p4
          do{
            i4++;
            p4 = primes[i4];
          }while( gcd( p4 - 1, P3 ) != 1);
          P4 = P3 * p4;

        }while(p4 < upper4); // end of do p4

        // find next p3 that makes p1*p2*p3 admissable
        do{
          i3++;
          p3 = primes[i3];
        }while( gcd( p3 - 1, P2 ) != 1 );
        P3 = P2 * p3;

      }while(p3 < upper3);  // end of do p3

      // find next p2 that makes p1*p2 admissable
      do{
        i2++;
        p2 = primes[i2];
      }while( gcd( p2 - 1, P1 ) != 1 );
      P2 = P1 * p2;

    }while(p2 < upper2);  // end of do p2

    // next prime p1
    i1 += num_threads;
    p1 = primes[i1];
    P1 = p1;
  }while(p1 < upper1);  // end of do p1

  output.close();
}
