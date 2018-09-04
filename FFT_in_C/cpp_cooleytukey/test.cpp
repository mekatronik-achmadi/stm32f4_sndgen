#include  <iostream>
#include  <complex>
#define MAX 200

using  namespace  std ;

int  log2 ( int  N )     // function to calculate the logarithm in base 2 of an integer
{
  int  k  =  N ,  i  =  0 ;
  while ( k )  {
    k  >>=  1 ;
    i ++ ;
  }
  return  i  -  1 ;
}

int  check ( int  n )     // used to check if the number of components of the input vector is a power of 2
{
  return  n  >  0  &&  ( n  &  ( n  -  1 ))  ==  0 ;
}

int  reverse ( int  N ,  int  n )     // calculate the reverse number of each integer n with respect to the maximum number N
{
  int  j ,  p  =  0 ;
  for ( j  =  1 ;  j  <=  log2 ( N );  j ++ )  {
    if ( n  &  ( 1  <<  ( log2 ( N )  -  j )))
      p  |=  1  <<  (j  -  1 );
  }
  return  p ;
}

void  sort ( complex < double > *  f1 ,  int  N )      // arrange the elements of the vector ordering them by reverse order
{
  complex < double >  f2 [ MAX ];
  for ( int  i  =  0 ;  i  <  N ;  i ++ )
    f2 [ i ]  =  f1 [ reverse ( N ,  i )];
  for ( int j  =  0 ;  j  <  N ;  j ++ )
    f1 [ j ]  =  f2 [ j ];
}

void  transform ( complex < double > *  f ,  int  N )      // calculate the transformed vector
{
  sort ( f ,  N );     // first order it with the reverse order
  complex < double >  W [ N  /  2 ];  // vector of unit zeroes.
                            // First N / 2-1 but generates error with next for loop
                           // because it tries to copy to an unallocated area "W [N / 2-1]"
  W [ 1 ]  =  polar ( 1.,  - 2.  *  M_PI  /  N );
  W [ 0 ]  =  1 ;
  for ( int  i  =  2 ;  i  <  N  /  2 ;  i ++ )
    W [ i ]  =  pow ( W [ 1 ],  i );
  int  n  =  1 ;
  int  a  =  N  /  2 ;
  for ( int j  =  0 ;  j  <  log2 ( N );  j ++ )  {
    for ( int  i  =  0 ;  i  <  N ;  i ++ )  {
      if ( ! ( i  &  n ))  {
        /* with each dou doubling step, the indices are used i alternatively taken in small groups of n, once yes and no. */
        complex < double >  temp  =  f [ i];
        complex < double >  Temp  =  W [( i  *  a )  %  ( n  *  a )]  *  f [ i  +  n ];
        f [ i ]  =  temp  +  Temp ;
        f [ i  +  n ]  =  temp  -  Temp ;
      }
    }
    n  *=  2 ;
    a  =  a  /  2 ;
  }
}

void  FFT ( complex < double > *  f ,  int  N ,  double  d )
{
  transform ( f ,  N );
  for ( int  i  =  0 ;  i  <  N ;  i ++ )
    f [ i ]  *=  d ;  // multiply the vector for the step in order to have the actual transformed vector
}

int  main ()
{
  int  n ;
  do  {
    cout  <<  "specifies the size of the vector, which is the power of 2"  <<  endl ;
    cin  >>  n ;
  }  while ( ! check ( n ));
  double  d ;
  cout  <<  "enter the size of the sampling step"  <<  endl ;
  cin  >>  d ;
  cout  <<  "sampling step ="  <<  d  << endl ;
  complex < double >  vec [ MAX ];
  cout  <<  "insert the sampling vector"  <<  endl ;
  for ( int  i  =  0 ;  i  <  n ;  i ++ )  {
    cout  <<  "insert the index component"  <<  i  <<  endl ;
    cin  >>  vec [ i ];
    cout  <<  "index"  << i <<  vec [ i ]   <<  endl ;
  }
  FFT ( vec ,  n ,  d );
  cout  <<  "transformed vector"  <<  endl ;
  for ( int  j  =  0 ;  j  <  n ;  j ++ )
    cout  <<  vec [ j ]  <<  endl ;
  return  0 ;
}
