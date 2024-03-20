/*tests.c*/

//
// Google test cases for our set class.
//
// Initial template: Prof. Joe Hummel
// Northwestern University
// CS 211
//

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <set>  // for comparing answers

using std::string;
using std::vector;
using std::pair;

#include "set.h"
#include "gtest/gtest.h"

// my tests 

TEST(myset, toPairs_empty_set)
{
  set<int> S;
  auto pairs = S.toPairs(-1);
  ASSERT_TRUE(pairs.empty());
}

TEST(myset, toPairs_set_with_one_element)
{
  set<int> S;
  S.insert(123);
  
  auto pairs = S.toPairs(-1);

  vector<pair<int, int>> expected = { {123, -1} };
  
  ASSERT_EQ(pairs.size(), (unsigned long) 1);
  ASSERT_EQ(pairs, expected);
}


TEST(myset, toPairs_with_multiple_elements)
{
  set<int> S;
  
  S.insert(30);
  S.insert(15); 
  S.insert(50); 
  S.insert(8); 
  S.insert(25); 
  S.insert(70); 
  S.insert(20); 
  S.insert(28); 
  S.insert(60);

  ASSERT_EQ(S.size(), 9);

  auto pairs = S.toPairs(-1);

  vector<pair<int, int>> expected = {
    {8, 15}, {15, -1}, {20, 25}, {25, -1}, {28, 30}, 
    {30, -1}, {50, -1}, {60, 70}, {70, -1}
  }; 

  ASSERT_EQ(pairs.size(), expected.size());
  
  for (size_t i = 0, n = expected.size(); i < n; ++i)
  {
    ASSERT_EQ(pairs[i], expected[i]);
  }
}

TEST(myset, toPairs_single_string_element)
{
  set<string> S;

  S.insert("Angola");

  ASSERT_EQ(S.size(), 1);

  auto pairs = S.toPairs("FIN");

  vector<pair<string, string>> expected = { {"Angola", "FIN"} };

  ASSERT_EQ(pairs.size(), expected.size());
  ASSERT_EQ(pairs[0], expected[0]);
}

TEST(myset, toPairs_multiple_string_elements)
{
  set<string> S;

  S.insert("Congo");
  S.insert("Brazil");
  S.insert("Djibouti");
  S.insert("Angola");
  S.insert("Eritrea");

  ASSERT_EQ(S.size(), 5);

  auto pairs = S.toPairs(" ");

  vector<pair<string, string>> expected = {
    {"Angola", "Brazil"},
    {"Brazil", "Congo"},
    {"Congo", " "},
    {"Djibouti", " "},
    {"Eritrea", " "}
  };

  ASSERT_EQ(pairs.size(), expected.size());
  
  for (size_t i = 0; i < expected.size(); ++i)
  {
    ASSERT_EQ(pairs[i], expected[i]);
  }
}


TEST(myset, foreach_with_empty_set)
{
  set<long> S;
  int count = 0;

  for (long x : S)
  {
    count++; 
  }

  ASSERT_EQ(count, 0);
}

TEST(myset, foreach_with_single_element_set)
{
  set<int> S;
  S.insert(42);  
  int count = 0;
  int val = 0;

  for (int x : S)
  {
    val = x;
    count++;    
  }

  ASSERT_EQ(count, 1);
  ASSERT_EQ(val, 42);
}

TEST(myset, foreach_with_increasing_multiple_elements_set)
{
  set<int> S;
  S.insert(10);
  S.insert(20);
  S.insert(30);

  std::vector<int> elements;
  
  for (int x : S)
  {
    elements.push_back(x);
  }

  std::vector<int> expected = {10, 20, 30};

  ASSERT_EQ(elements.size(), expected.size());
  
  for (size_t i = 0; i < expected.size(); ++i)
  {
    ASSERT_EQ(elements[i], expected[i]);
  }
}

TEST(myset, foreach_with_unordered_multiple_elements_set)
{
  set<int> S;

  std::vector<int> expected = {33, 21, 19, 35, 21};
  std::vector<int> sorted_expected = expected;
  // removing duplicates
  std::set<int> s;
  unsigned size = sorted_expected.size();
  for( unsigned i = 0; i < size; ++i ) s.insert( sorted_expected[i] );
  sorted_expected.assign( s.begin(), s.end() );

  for (int i : expected) {
    S.insert(i);
  }

  std::vector<int> elements; 
  
  for (int x : S)
  {
    elements.push_back(x);
  }

  ASSERT_EQ(elements.size(), sorted_expected.size());
  
  for (size_t i = 0; i < sorted_expected.size(); ++i)
  {
    ASSERT_EQ(elements[i], sorted_expected[i]);
  }
}

TEST(myset, foreach_with_single_string_element)
{
  set<string> S;
  S.insert("Hello");
  int count = 0;
  string value;

  for (const string& x : S)
  {
    value = x;
    count++;    
  }

  ASSERT_EQ(count, 1);
  ASSERT_EQ(value, "Hello");
}

TEST(myset, foreach_with_multiple_string_elements)
{
  set<string> S;
  
  S.insert("apple");
  S.insert("banana");
  S.insert("cherry");

  std::vector<string> elements;
  
  for (const string& x : S)
  {
    elements.push_back(x);
  }

  std::vector<string> expected = {"apple", "banana", "cherry"};

  ASSERT_EQ(elements.size(), expected.size());
  
  for (size_t i = 0; i < expected.size(); ++i)
  {
    ASSERT_EQ(elements[i], expected[i]);
  }
}

TEST(myset, foreach_stress_test_with_strictly_increasing_nums)
{
  set<int> S;
  const int N_ELEMS = 1000000;
  for (int i = 0; i < N_ELEMS; ++i) {
    S.insert(i);
  }

  // check set size
  ASSERT_EQ(S.size(), N_ELEMS);

  // check values
  int count = 0;
  for (int x : S) {
    ASSERT_EQ(x, count);
    count++;
  }

  ASSERT_EQ(count, N_ELEMS);
}

TEST(myset, foreach_stress_test_with_random_nums)
{
  set<int> S;
  const int N_ELEMS = 1000000;

  // populate a vector with random numbers
  // code below is from https://stackoverflow.com/a/23143753
  std::random_device rnd_device;
  std::mt19937 mersenne_engine {rnd_device()}; 
  std::uniform_int_distribution<int> dist {1, 52};
  auto gen = [&dist, &mersenne_engine](){ 
    return dist(mersenne_engine); 
  };
  vector<int> vec(N_ELEMS);
  generate(begin(vec), end(vec), gen);

  // popualate set
  for (int i = 0; i < N_ELEMS; ++i) {
    S.insert(vec[i]);
  }

  // create sorted vector
  std::vector<int> sorted_expected = vec;
  // removing duplicates
  std::set<int> s;
  unsigned size = sorted_expected.size();
  for( unsigned i = 0; i < size; ++i ) s.insert( sorted_expected[i] );
  sorted_expected.assign( s.begin(), s.end() );

  // check set size
  ASSERT_EQ(S.size(), sorted_expected.size());

  // check values
  int count = 0;
  for (int x : S) {
    ASSERT_EQ(x, sorted_expected[count]);
    count++;
  }
}

// intructor's tests 

TEST(myset, empty_set)
{
  set<int> S;

  ASSERT_EQ(S.size(), 0);
}

TEST(myset, set_with_one)
{
  set<int> S;

  ASSERT_EQ(S.size(), 0);

  S.insert(123);

  ASSERT_EQ(S.size(), 1);

  ASSERT_TRUE(S.contains(123));
  ASSERT_TRUE(S[123]);

  ASSERT_FALSE(S.contains(100));
  ASSERT_FALSE(S[100]);
  ASSERT_FALSE(S.contains(200));
  ASSERT_FALSE(S[200]);
}

TEST(myset, set_with_four_strings)
{
  set<string> S;

  ASSERT_EQ(S.size(), 0);

  S.insert("banana");
  S.insert("apple");
  S.insert("chocolate");
  S.insert("pear");

  ASSERT_EQ(S.size(), 4);

  ASSERT_TRUE(S.contains("pear"));
  ASSERT_TRUE(S["banana"]);
  ASSERT_TRUE(S.contains("chocolate"));
  ASSERT_TRUE(S["apple"]);

  ASSERT_FALSE(S.contains("Apple"));
  ASSERT_FALSE(S["carmel"]);
  ASSERT_FALSE(S.contains("appl"));
  ASSERT_FALSE(S["chocolatee"]);
}

class Movie
{
public:
  string Title;
  int    ID;
  double Revenue;

  Movie(string title, int id, double revenue)
    : Title(title), ID(id), Revenue(revenue)
  { }

  bool operator<(const Movie& other)
  {
    if (this->Title < other.Title)
      return true;
    else
      return false;
  }
};

TEST(myset, set_with_movies)
{
  set<Movie> S;

  ASSERT_EQ(S.size(), 0);

  Movie Sleepless("Sleepless in Seattle", 123, 123456789.00);
  S.insert(Sleepless);

  Movie Matrix("The Matrix", 456, 400000000.00);
  S.insert(Matrix);

  Movie AnimalHouse("Animal House", 789, 1000000000.00);
  S.insert(AnimalHouse);

  ASSERT_EQ(S.size(), 3);

  vector<Movie> V = S.toVector();

  ASSERT_EQ(V[0].Title, "Animal House");
  ASSERT_EQ(V[1].Title, "Sleepless in Seattle");
  ASSERT_EQ(V[2].Title, "The Matrix");
}

TEST(myset, set_from_class_with_nine)
{
  set<int> S;

  vector<int> V = { 22, 11, 49, 3, 19, 35, 61, 30, 41 };

  for (auto x : V)
    S.insert(x);

  ASSERT_EQ(S.size(), (int) V.size());

  for (auto x : V) {
    ASSERT_TRUE(S.contains(x));
    ASSERT_TRUE(S[x]);
  }

  ASSERT_FALSE(S.contains(0));
  ASSERT_FALSE(S[0]);
  ASSERT_FALSE(S.contains(2));
  ASSERT_FALSE(S[2]);
  ASSERT_FALSE(S.contains(4));
  ASSERT_FALSE(S[4]);
  ASSERT_FALSE(S.contains(29));
  ASSERT_FALSE(S[31]);
  ASSERT_FALSE(S.contains(40));
  ASSERT_FALSE(S[42]);
}

TEST(myset, set_no_duplicates)
{
  set<int> S;

  vector<int> V = { 22, 11, 49, 3, 19, 35, 61, 30, 41 };

  for (auto x : V)
    S.insert(x);

  // try to insert them all again:
  for (auto x : V)
    S.insert(x);

  ASSERT_EQ(S.size(), (int) V.size());

  for (auto x : V) {
    ASSERT_TRUE(S.contains(x));
    ASSERT_TRUE(S[x]);
  }
}

TEST(myset, toVector)
{
  set<int> S;

  vector<int> V = { 22, 11, 49, 3, 19, 35, 61, 30, 41 };

  for (auto x : V)
    S.insert(x);

  ASSERT_EQ(S.size(), (int) V.size());

  vector<int> V2 = S.toVector();

  ASSERT_EQ(V2.size(), V.size());

  std::sort(V.begin(), V.end());

  //
  // V and V2 should have the same elements in 
  // the same order:
  //
  auto iterV = V.begin();
  auto iterV2 = V2.begin();

  while (iterV != V.end()) {
    ASSERT_EQ(*iterV, *iterV2);

    iterV++;
    iterV2++;
  }
}

TEST(myset, copy_empty)
{
  set<int> S1;

  {
    //
    // create a new scope, which will trigger destructor:
    //
    set<int> S2 = S1;  // this will call copy constructor:

    S1.insert(123);  // this should have no impact on S2:
    S1.insert(100);
    S1.insert(150);

    ASSERT_EQ(S2.size(), 0);

    vector<int> V2 = S2.toVector();

    ASSERT_EQ((int) V2.size(), 0);
  }
}

TEST(myset, copy_constructor)
{
  set<int> S1;

  vector<int> V = { 22, 11, 49, 3, 19, 35, 61, 30, 41 };

  for (auto x : V)
    S1.insert(x);

  ASSERT_EQ(S1.size(), (int) V.size());

  {
    //
    // create a new scope, which will trigger destructor:
    //
    set<int> S2 = S1;  // this will call copy constructor:

    S1.insert(123);  // this should have no impact on S2:
    S1.insert(100);
    S1.insert(150);

    ASSERT_EQ(S2.size(), (int) V.size());

    vector<int> V2 = S2.toVector();

    ASSERT_EQ(V2.size(), V.size());

    std::sort(V.begin(), V.end());

    //
    // V and V2 should have the same elements in 
    // the same order:
    //
    auto iterV = V.begin();
    auto iterV2 = V2.begin();

    while (iterV != V.end()) {
      ASSERT_EQ(*iterV, *iterV2);

      iterV++;
      iterV2++;
    }

    S2.insert(1000);  // this should have no impact on S1:
    S2.insert(2000);
    S2.insert(3000);
    S2.insert(4000);
    S2.insert(5000);

    V.push_back(123);
    V.push_back(100);
    V.push_back(150);
  }

  //
  // the copy was just destroyed, the original set
  // should still be the same as it was earlier:
  //
  ASSERT_EQ(S1.size(), (int) V.size());

  vector<int> V2 = S1.toVector();

  ASSERT_EQ(V2.size(), V.size());

  std::sort(V.begin(), V.end());

  //
  // V and V2 should have the same elements in 
  // the same order:
  //
  auto iterV = V.begin();
  auto iterV2 = V2.begin();

  while (iterV != V.end()) {
    ASSERT_EQ(*iterV, *iterV2);

    iterV++;
    iterV2++;
  }
}

TEST(myset, find_empty)
{
  set<int> S;

  auto iter = S.find(22);
  ASSERT_TRUE(iter == S.end());
}

TEST(myset, find_one)
{
  set<int> S;

  S.insert(1234);

  auto iter = S.find(123);
  ASSERT_TRUE(iter == S.end());

  iter = S.find(1234);
  if (iter == S.end()) {  // this should not happen:
    ASSERT_TRUE(false); // fail:
  }

  ASSERT_EQ(*iter, 1234);

  iter = S.find(1235);
  ASSERT_TRUE(iter == S.end());
}

TEST(myset, find_with_set_from_class)
{
  set<int> S;

  vector<int> V = { 22, 11, 49, 3, 19, 35, 61, 30, 41 };

  for (auto x : V)
    S.insert(x);

  ASSERT_EQ(S.size(), (int) V.size());

  //
  // make sure we can find each of the values we inserted:
  //
  for (auto x : V) {
    auto iter = S.find(x);

    if (iter == S.end()) {  // this should not happen:
      ASSERT_TRUE(false); // fail:
    }

    ASSERT_EQ(*iter, x);
  }

  //
  // these searches should all fail:
  //
  auto iter = S.find(0);
  ASSERT_TRUE(iter == S.end());

  iter = S.find(-1);
  ASSERT_TRUE(iter == S.end());

  iter = S.find(1);
  ASSERT_TRUE(iter == S.end());

  iter = S.find(4);
  ASSERT_TRUE(iter == S.end());

  iter = S.find(34);
  ASSERT_TRUE(iter == S.end());

  iter = S.find(36);
  ASSERT_TRUE(iter == S.end());
}

TEST(myset, stress_test)
{
  set<long long> S;
  std::set<long long> C;

  long long N = 1000000;

  //
  // setup random number generator so tree will
  // be relatively balanced given insertion of
  // random numbers:
  //
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<long long> distrib(1, N * 100);  // inclusive

  vector<long long> V;  // collect a few values for searching:
  int count = 0;

  while (S.size() != N) {

    long long x = distrib(gen);

    S.insert(x);
    C.insert(x);

    count++;
    if (count == 1000) { // save every 1,000th value:

      V.push_back(x);
      count = 0;
    }
  }

  ASSERT_EQ(S.size(), N);

  for (auto x : V) {
    ASSERT_TRUE(S.contains(x));
  }

  ASSERT_FALSE(S.contains(0));
  ASSERT_FALSE(S.contains(-1));

  //
  // now let's compare our set to C++ set:
  //
  V.clear();
  V = S.toVector();

  ASSERT_EQ(V.size(), C.size());
  ASSERT_EQ(S.size(), (int) C.size());

  int i = 0;

  for (auto x : C) {
    ASSERT_EQ(V[i], x);
    i++;
  }
}