/*
E3.13
[13] Create a program to find all the prime numbers between 1 and 100.
There is a classic method for doing this, called the “Sieve of
Eratosthenes.” If you don’t know that method, get on the Web and look
it up. Write your program using this method.


This program was somewhat hard to implement because of the syntax of the v.erase() method.

30 Oct 2024
@jmerort
*/

#include <iostream>
#include <vector>

int main()
{
    std::vector <int> nums = {};
    int counter = 0;

    //fill a vector with numbers from 2 to 100
    for (int i = 2; i <= 100; ++i)
        nums.push_back(i);
    
    //remove the elements that are multiples of primes, starting with 2
    for (int p = 0; p < nums.size(); ++p) //this variable will iterate over only primes, as the non-primes will be removed inside the loop
    {
        for (int u = nums[p]; u < nums.size(); ++u) //this variable iterates over every remaining memebr of num starting from the current prime
        {
            if (nums[u] % nums[p] == 0 && nums[u] > nums[p])
                nums.erase(nums.begin()+u); 
        }
    }

    //write results
    std::cout << "The prime numbers between 2 and 100 are:\n";
    for (int x = 0; x < nums.size(); ++x)
        std::cout << nums[x] << "\n";


    return 0;
}