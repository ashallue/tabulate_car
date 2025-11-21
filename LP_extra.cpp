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
  //cout << "upper1 = " << upper1 << "\n";

  // start p1 at the prime corresponding to thread number
  // update: all threads start at 0, use num_admissable to determine if inner loop work performed or not
  int num_admissable = 0;
  
  i1 = 0;
  p1 = primes[i1];
  P1 = p1;
  do{

    // compute L1
    L1 = p1 - 1;

    // start off p2 at the next prime
    i2 = i1 + 1;

    // also need to compute the corresponding upper bound: (B/p1)^{1/4}
    upper2 = find_upper(B, p1, 4);

    // finding the start index for p2
    p2 = primes[i2];
    // check admissability, bump ahead until found
    while( gcd( p2 - 1, P1) != 1){
      i2++;
      p2 = primes[i2];
    }
    P2 = P1 * p2;
    
    do{
      // check threading
      // only do the p3 work if correct thread
      if(num_admissable % num_threads == thread){

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

      //cout << "past num_admissable check, p1 = " << p1 << " p2 = " << p2 << " p3 = " << p3 << "\n";

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
      } // end if threading correct

      // find next p2 that makes p1*p2 admissable
      do{
        i2++;
        p2 = primes[i2];
      }while( gcd( p2 - 1, P1 ) != 1 );
      P2 = P1 * p2;

      num_admissable++;

    }while(p2 < upper2);  // end of do p2

    // next prime p1
    i1 += 1;
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

  // threading based on the number of admissable pre-products
  long num_admissable = 0;

  // nested for loops
  // compute first upper bound as B^{1/6}
  upper1 = find_upper(B, 1, 6);
  //cout << "upper1 = " << upper1 << "\n";

  // start p1 at 3
  i1 = 0;
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
      // check threading
      // only do the p3 work if correct thread
      if(num_admissable % num_threads == thread){
      //cout << "correct thread with num_admissable = " << num_admissable << "\n";

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

        //if(P4 == 1148581) cout << "\n1148581 found in thread " << thread << "\n\n";

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

             
            //if(P5 == 112781131) cout << "Inner loop with p1 = " << p1 << " p2 = " << p2 << " p3 = " << p3 << " p4 = " << p4 << " q = " << q << "\n";

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
      } // end if correct thread.  If yes, work above done.  If not, find next admissable p2
      // find next p2 that makes p1*p2 admissable
      do{
        i2++;
        p2 = primes[i2];
      }while( gcd( p2 - 1, P1 ) != 1 );
      P2 = P1 * p2;

      // increment num_admissable counter
      num_admissable++;

    }while(p2 < upper2);  // end of do p2

    // next prime p1
    i1 ++;
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
  //cout << "upper1 = " << upper1 << "\n";

  // start p1 at the prime corresponding to thread number
  // Update: new threading.  All threads consider same primes, but only enter inner loop
  // if num_admissable is in a certain class
  long num_admissable = 0;  

  // timings test, start not at p1 = 3, but at p1 a large prime
  //i1 = 320;

  i1 = 0;
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

          // check threading
          // only do the p5 work if correct thread
          if(num_admissable % num_threads == thread){

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
              // update L6
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
          } // end if admissalbe in a certain thread
 
          // find next p4
          do{
            i4++;
            p4 = primes[i4];
          }while( gcd( p4 - 1, P3 ) != 1);
          P4 = P3 * p4;
        
          num_admissable++;
 
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
    i1 += 1;
    p1 = primes[i1];
    P1 = p1;
  }while(p1 < upper1);  // end of do p1

  output.close();
}


// threaded version of cars8
void LargePreproduct::cars8_threaded(string cars_file, long thread, long num_threads){
  //setup file
  ofstream output;
  output.open(cars_file);

  // primes out of the primes index, their indices
  long p1, p2, p3, p4, p5, p6, q;
  long i1, i2, i3, i4, i5, i6, i7;
  // lower bounds are given in terms of index, uppers in terms of values
  long lower_index;
  long upper1, upper2, upper3, upper4, upper5, upper6, upper7;

  // keep running computation of P and lcm_p|P p-1
  bigint P1, P2, P3, P4, P5, P6, P7;
  bigint L1, L2, L3, L4, L5, L6, L7;
  long g;

  vector<long> rs;

  // nested for loops
  // compute first upper bound as B^{1/8}
  upper1 = find_upper(B, 1, 8);
  //cout << "upper1 = " << upper1 << "\n";

  // start p1 at the prime corresponding to thread number
  // Update: new threading.  All threads consider same primes, but only enter inner loop
  // if num_admissable is in a certain class
  long num_admissable = 0;  

  i1 = 0;
  p1 = primes[i1];
  P1 = p1;
  do{

    // compute L1
    L1 = p1 - 1;

    // take p2 to be next prime after p1
    i2 = i1 + 1;

    // also need to compute the corresponding upper bound: (B/p1)^{1/7}
    upper2 = find_upper(B, p1, 7);
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

      // upper bound is (B/p1p2)^{1/6}
      upper3 = find_upper(B, P2, 6);
      
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

        // upper bound is (B/p1p2p3)^{1/5}
        upper4 = find_upper(B, P3, 5);

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

          // take p5 to be next prime, compute upper bound as (B/p1p2p3p4)^{1/4}
          i5 = i4 + 1;
          upper5 = find_upper(B, P4, 4);

          // check admissability to find start index for p5
          p5 = primes[i5];
          while( gcd(p5 - 1, P4) != 1){
            i5++;
            p5 = primes[i5];
          }
          P5 = P4 * p5; 

          do{

            // check threading
            // only do the p6 work if correct thread
            if(num_admissable % num_threads == thread){

            // update L5
            L5 = L4 * (p5 - 1);
            g = gcd(L4, p5 - 1);
            L5 = L5 / g;

            // if p1 * p2 * p3 * p4 * p5^2 > X take i6 = i5 + 1.  Otherwise X / p1p2p3p4p5
            if(P5 * p5 > X){
              lower_index = i5 + 1;
            }else{
              lower_index = find_index_lower( X / P5 );
            }
            i6 = lower_index;

            // upper bound is (B/p1p2p3p4p5)^{1/3}
            upper6 = find_upper(B, P5, 3);

            // find start index for p6, discarding choices not admissable
            p6 = primes[i6];
            while( gcd(p6 - 1, P5) != 1 ){
              i6++;
              p6 = primes[i6];
            }
            P6 = P5 * p6;

            do{
              // update L6
              L6 = L5 * (p6 - 1);
              g = gcd(L5, p6 - 1);
              L6 = L6 / g;

              // lower bound for q is just the previous prime, upper is (B/p1p2p3p4p5p6)^{1/2}
              upper7 = find_upper(B, P6, 2);
        
              // finding the start index and prime for q
              i7 = i6 + 1;
              q = primes[i7];
              // check admissability, bump ahead until found
              while( gcd( q - 1, P6 ) != 1 ){
                i7++;
                q = primes[i7];
              }
              P7 = P6 * q;

              do{
                // update L7
                L7 = L6 * (q - 1);
                g = gcd(L6, q - 1);
                L7 = L7 / g;

                if(P7 == 5140718765) cout << "Inner loop with p1 = " << p1 << " p2 = " << p2 << " p3 = " << p3 << " p4 = " << p4 << " p5 = " << p5 << " q = " << q << "\n";

                // complicated inner loop work that finds r's that make carmichaels
                // clears rs vector and refills it
                inner_loop_work(P7, q, L7, rs);

                // write to file
                for(long i = 0; i < rs.size(); i++){
                  // note this next line might attempt to print a bigint and faile
                  output << P7 * rs[i] << " ";
                  output << p1 << " " << p2 << " " << p3 << " " << p4 << " " << p5 << " " << p6 << " " << q << " " << rs[i] << "\n";
                }

                // find next q that makes preproduct admissable
                do{
                  i7++;
                  q = primes[i7];
                }while( gcd( q - 1, P6 ) != 1 );
                P7 = P6 * q;

              } while(q < upper7); // end of do q

              // find next p6
              do{
                i6++;
                p6 = primes[i6];
              }while( gcd( p6 - 1, P5 ) != 1 );
              P6 = P5 * p6;

            }while(p6 < upper6); // end of do p6
            } // end if admissalbe in a certain thread
 
            // find next p5
            do{
              i5++;
              p5 = primes[i5];
            }while( gcd( p5 - 1, P4 ) != 1);
            P5 = P4 * p5;

            num_admissable++;

          }while(p5 < upper5);  // end of do p5

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
    i1 += 1;
    p1 = primes[i1];
    P1 = p1;
  }while(p1 < upper1);  // end of do p1

  output.close();
}


// threaded version of cars9
void LargePreproduct::cars9_threaded(string cars_file, long thread, long num_threads){
  //setup file
  ofstream output;
  output.open(cars_file);

  // primes out of the primes index, their indices
  long p1, p2, p3, p4, p5, p6, p7, q;
  long i1, i2, i3, i4, i5, i6, i7, i8;
  // lower bounds are given in terms of index, uppers in terms of values
  long lower_index;
  long upper1, upper2, upper3, upper4, upper5, upper6, upper7, upper8;

  // keep running computation of P and lcm_p|P p-1
  bigint P1, P2, P3, P4, P5, P6, P7, P8;
  bigint L1, L2, L3, L4, L5, L6, L7, L8;
  long g;

  vector<long> rs;

  // nested for loops
  // compute first upper bound as B^{1/9}
  upper1 = find_upper(B, 1, 9);
  //cout << "upper1 = " << upper1 << "\n";

  // start p1 at the prime corresponding to thread number
  // Update: new threading.  All threads consider same primes, but only enter inner loop
  // if num_admissable is in a certain class
  long num_admissable = 0;  

  i1 = 0;
  p1 = primes[i1];
  P1 = p1;
  do{

    // compute L1
    L1 = p1 - 1;

    // take p2 to be next prime after p1
    i2 = i1 + 1;

    // also need to compute the corresponding upper bound: (B/p1)^{1/8}
    upper2 = find_upper(B, p1, 8);
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

      // upper bound is (B/p1p2)^{1/7}
      upper3 = find_upper(B, P2, 7);
      
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

        // upper bound is (B/p1p2p3)^{1/6}
        upper4 = find_upper(B, P3, 6);

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

          // start p5 as next prime after p4, upper bound is (B/p1p2p3p4)^{1/5}
          i5 = i4 + 1;
          upper5 = find_upper(B, P4, 5);

          // find start index for p5, making sure it is admissable
          p5 = primes[i5];
          while( gcd(p5 - 1, P4) != 1){
            i5++;
            p5 = primes[i5];
          }
          P5 = P4 * p5;

          do{
            // update L5
            L5 = L4 * (p5 - 1);
            g = gcd(L4, p5 - 1);
            L5 = L5 / g;

            // start p6 as next prime after p5, upper bound is (B/P5)^{1/4}
            i6 = i5 + 1;
            upper6 = find_upper(B, P5, 4);

            // find start index for p6, checking admissability
            p6 = primes[i6];
            while( gcd(p6 - 1, P5) != 1){
              i6++;
              p6 = primes[i6];
            }
            P6 = P5 * p6;

            do{
              // check threading
              // only do the p5 work if correct thread
              if(num_admissable % num_threads == thread){

              // update L6
              L6 = L5 * (p6 - 1);
              g = gcd(L5, p6 - 1);
              L6 = L6 / g;

              // if p1 * p2 * p3 * p4 * p5 * p6^2 > X take i7 = i6 + 1.  Otherwise X / p1p2p3p4p5p6
              if(P6 * p6 > X){
                lower_index = i6 + 1;
              }else{
                lower_index = find_index_lower( X / P6 );
              }
              i7 = lower_index;

              // upper bound is (B/p1p2p3p4p5p6)^{1/3}
              upper7 = find_upper(B, P6, 3);

              // find start index for p7, discarding choices not admissable
              p7 = primes[i7];
              while( gcd(p7 - 1, P6) != 1 ){
                i7++;
                p7 = primes[i7];
              }
              P7 = P6 * p7;

              do{
                // update L7
                L7 = L6 * (p7 - 1);
                g = gcd(L6, p7 - 1);
                L7 = L7 / g;

                // lower bound for q is just the previous prime, upper is (B/p1p2p3p4p5p6p7)^{1/2}
                upper8 = find_upper(B, P7, 2);
        
                // finding the start index and prime for q
                i8 = i7 + 1;
                q = primes[i8];
                // check admissability, bump ahead until found
                while( gcd( q - 1, P7 ) != 1 ){
                  i8++;
                  q = primes[i8];
                }
                P8 = P7 * q;

                do{
                  // update L8
                  L8 = L7 * (q - 1);
                  g = gcd(L7, q - 1);
                  L8 = L8 / g;

                  //cout << "Inner loop with p1 = " << p1 << " p2 = " << p2 << " p3 = " << p3 << " p4 = " << p4 << " p5 = " << p5 << " q = " << q << "\n";

                  // complicated inner loop work that finds r's that make carmichaels
                  // clears rs vector and refills it
                  inner_loop_work(P8, q, L8, rs);

                  // write to file
                  for(long i = 0; i < rs.size(); i++){
                    // note this next line might attempt to print a bigint and faile
                    output << P8 * rs[i] << " ";
                    output << p1 << " " << p2 << " " << p3 << " " << p4 << " " << p5 << " " << p6 << " " << p7 << " " << q << " " << rs[i] << "\n";
                  }

                  // find next q that makes P7 * q admissable
                  do{
                    i8++;
                    q = primes[i8];
                  }while( gcd( q - 1, P7 ) != 1 );
                  P8 = P7 * q;

                } while(q < upper8); // end of do q

                // find next p7
                do{
                  i7++;
                  p7 = primes[i7];
                }while( gcd( p7 - 1, P6 ) != 1 );
                P7 = P6 * p7;

              }while(p7 < upper7); // end of do p7
              } // end if admissalbe in a certain thread
        
              // find next p6
              do{
                i6++;
                p6 = primes[i6];
              }while( gcd( p6 - 1, P5 ) != 1 );      
              P6 = P5 * p6;

            }while(p6 < upper6); // end of do p6
             
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
        
          num_admissable++;
 
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
    i1 += 1;
    p1 = primes[i1];
    P1 = p1;
  }while(p1 < upper1);  // end of do p1

  output.close();
}
