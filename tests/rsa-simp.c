// C program for RSA asymmetric cryptographic 
// algorithm. For demonstration values are 
// relatively small compared to practical 
// application .
// This code is contributed by Akash Sharan.
// https://www.geeksforgeeks.org/rsa-algorithm-cryptography/

#include<stdio.h> 
#include<math.h> 
  
// Returns gcd of a and b 
int gcd(int a, int h) 
{ 
    int temp; 
    while (1) 
    { 
        temp = a%h; 
        if (temp == 0) 
          return h; 
        a = h; 
        h = temp; 
    } 
} 
  
// Code to demonstrate RSA algorithm 
int main() 
{ 
    // Two random prime numbers 
    double p = 3; 
    double q = 7; 
  
    // First part of public key: 
    double n = p*q; 
  
    // Finding other part of public key. 
    // e stands for encrypt 
    double e = 2; 
    double phi = (p-1)*(q-1); 
    while (e < phi) 
    { 
        // e must be co-prime to phi and 
        // smaller than phi. 
        if (gcd(e, phi)==1) 
            break; 
        else
            e++; 
    } 
  
    // Private key (d stands for decrypt) 
    // choosing d such that it satisfies 
    // d*e = 1 + k * totient 
    int k = 2;  // A constant value 
    double d = (1 + (k*phi))/e; 

    printf("Modulo = %lf \n", n); 
    printf("Public key = %lf \n", e); 
    printf("Private key = %lf \n", d); 
  
    // Message to be encrypted 
    double msg = 12; 
  
    printf("Message data = %lf \n", msg); 
  
    // Encryption c = (msg ^ e) % n 
    double c = pow(msg, e); 
    c = fmod(c, n); 
    printf("Encrypted data = %lf \n", c); 
  
    // Decryption m = (c ^ d) % n 
    double m = pow(c, d); 
    m = fmod(m, n); 
    printf("Original Message Sent = %lf \n", m); 
  
    /*
Modulo = 21.000000 
Public key = 5.000000 
Private key = 5.000000 
Message data = 12.000000 
Encrypted data = 3.000000 
Original Message Sent = 12.000000
    */

    return 0; 
} 

