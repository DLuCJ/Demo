//
//	                 FFT library
//
//  (one-dimensional complex and real FFTs for array 
//  lengths of 2^n)
//
//	Author: Toth Laszlo (tothl@inf.u-szeged.hu)
//  
//	Research Group on Artificial Intelligence
//  H-6720 Szeged, Aradi vertanuk tere 1, Hungary
//	
//	Last modified: 97.05.29
/////////////////////////////////////////////////////////

#include <math.h>
#include <stdlib.h>
#define pi 3.1415926535897932


/////////////////////////////////////////////////////////
// Sorensen in-place split-radix FFT for real values
// data: array of doubles:
// re(0),re(1),re(2),...,re(size-1)
// 
// output:
// re(0),re(1),re(2),...,re(size/2),im(size/2-1),...,im(1)
// normalized by array length
//
// Source: 
// Sorensen et al: Real-Valued Fast Fourier Transform Algorithms,
// IEEE Trans. ASSP, ASSP-35, No. 6, June 1987

void realfft_split(double *data,long n){

  long i,j,k,i5,i6,i7,i8,i0,id,i1,i2,i3,i4,n2,n4,n8;
  double t1,t2,t3,t4,t5,t6,a3,ss1,ss3,cc1,cc3,a,e,sqrt2;
  
  sqrt2=sqrt(2.0);
  n4=n-1;
  
  //data shuffling
      for (i=0,j=0,n2=n/2; i<n4 ; i++){
	  if (i<j){
				t1=data[j];
				data[j]=data[i];
				data[i]=t1;
				}
	  k=n2;
	  while (k<=j){
				j-=k;
				k>>=1;	
				}
	  j+=k;
      }
	
/*----------------------*/
	
	//length two butterflies	
	i0=0;
	id=4;
   do{
       for (; i0<n4; i0+=id){ 
			i1=i0+1;
			t1=data[i0];
			data[i0]=t1+data[i1];
			data[i1]=t1-data[i1];
		}
	   id<<=1;
       i0=id-2;
       id<<=1;
    } while ( i0<n4 );

   /*----------------------*/
   //L shaped butterflies
n2=2;
for(k=n;k>2;k>>=1){  
	n2<<=1;
	n4=n2>>2;
	n8=n2>>3;
	e = 2*pi/(n2);
	i1=0;
	id=n2<<1;
	do{ 
	    for (; i1<n; i1+=id){
			i2=i1+n4;
			i3=i2+n4;
			i4=i3+n4;
			t1=data[i4]+data[i3];
			data[i4]-=data[i3];
			data[i3]=data[i1]-t1;
			data[i1]+=t1;
			if (n4!=1){
				i0=i1+n8;
				i2+=n8;
				i3+=n8;
				i4+=n8;
				t1=(data[i3]+data[i4])/sqrt2;
				t2=(data[i3]-data[i4])/sqrt2;
				data[i4]=data[i2]-t1;
				data[i3]=-data[i2]-t1;
				data[i2]=data[i0]-t2;
				data[i0]+=t2;
			}
	     }
		 id<<=1;
	     i1=id-n2;
	     id<<=1;
	  } while ( i1<n );
	a=e;
	for (j=2; j<=n8; j++){  
	      a3=3*a;
	      cc1=cos(a);
	      ss1=sin(a);
	      cc3=cos(a3);
	      ss3=sin(a3);
	      a=j*e;
	      i=0;
	      id=n2<<1;
	      do{
		   for (; i<n; i+=id){  
			  i1=i+j-1;
			  i2=i1+n4;
			  i3=i2+n4;
			  i4=i3+n4;
			  i5=i+n4-j+1;
			  i6=i5+n4;
			  i7=i6+n4;
			  i8=i7+n4;
			  t1=data[i3]*cc1+data[i7]*ss1;
			  t2=data[i7]*cc1-data[i3]*ss1;
			  t3=data[i4]*cc3+data[i8]*ss3;
			  t4=data[i8]*cc3-data[i4]*ss3;
			  t5=t1+t3;
			  t6=t2+t4;
			  t3=t1-t3;
			  t4=t2-t4;
			  t2=data[i6]+t6;
			  data[i3]=t6-data[i6];
			  data[i8]=t2;
			  t2=data[i2]-t3;
			  data[i7]=-data[i2]-t3;
			  data[i4]=t2;
			  t1=data[i1]+t5;
			  data[i6]=data[i1]-t5;
			  data[i1]=t1;
			  t1=data[i5]+t4;
			  data[i5]-=t4;
			  data[i2]=t1;
		     }
		   id<<=1;
		   i=id-n2;
		   id<<=1;
		 } while(i<n);
	   }
      }

	//division with array length
   for(i=0;i<n;i++) data[i]/=n;
}


/////////////////////////////////////////////////////////
// Sorensen in-place split-radix FFT for real values
// data: array of doubles:
// re(0),re(1),re(2),...,re(size-1)
// 
// output:
// re(0),re(size/2),re(1),im(1),re(2),im(2),...,re(size/2-1),im(size/2-1)
// normalized by array length
//
// Source: 
// Source: see the routines it calls ...

void realfft_split_unshuffled(double *data,long n){

	double *data2;
	long i,j;

	realfft_split(data,n);
	//unshuffling - not in-place
	data2=(double *)malloc(n*sizeof(double));
	j=n/2;
	data2[0]=data[0];
	data2[1]=data[j];
	for(i=1;i<j;i++) {data2[i+i]=data[i];data2[i+i+1]=data[n-i];}
	for(i=0;i<n;i++) data[i]=data2[i];
	free(data2);
}
