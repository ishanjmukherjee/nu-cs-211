/*tests.c*/

//
// Google test cases for our multiset class.
//
// Ishan Mukherjee
// CS 211
// Northwestern University
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

#include "multiset.h"
#include "gtest/gtest.h"

// example class to test support for nontrivial objects
// moved from instructor's tests to top of tests file

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

// my tests 

TEST(mymultiset, toPairs_empty_multiset)
{
  multiset<int> S;
  auto pairs = S.toPairs(-1);
  ASSERT_TRUE(pairs.empty());
}

TEST(mymultiset, toPairs_multiset_with_one_element)
{
  multiset<int> S;
  S.insert(123);
  
  auto pairs = S.toPairs(-1);

  vector<pair<int, int>> expected = { {123, -1} };
  
  ASSERT_EQ(pairs.size(), (unsigned long) 1);
  ASSERT_EQ(pairs, expected);
}

TEST(mymultiset, toPairs_with_multiple_elements)
{
  multiset<int> S;
  
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

TEST(mymultiset, toPairs_single_string_element)
{
  multiset<string> S;

  S.insert("Angola");

  ASSERT_EQ(S.size(), 1);

  auto pairs = S.toPairs("FIN");

  vector<pair<string, string>> expected = { {"Angola", "FIN"} };

  ASSERT_EQ(pairs.size(), expected.size());
  ASSERT_EQ(pairs[0], expected[0]);
}

TEST(mymultiset, toPairs_multiple_string_elements)
{
  multiset<string> S;

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

TEST(mymultiset, foreach_with_empty_multiset)
{
  multiset<long> S;
  int count = 0;

  for (long x : S)
  {
    count++; 
  }

  ASSERT_EQ(count, 0);
}

TEST(mymultiset, foreach_with_single_element_multiset)
{
  multiset<int> S;
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

TEST(mymultiset, foreach_with_increasing_ints_no_duplicates)
{
  multiset<int> S;

  std::vector<int> expected = {0, 10, 20, 30};
  sort(expected.begin(), expected.end());

  for (int i : expected) {
    S.insert(i);
  }

  std::vector<int> elements;
  
  for (int x : S)
  {
    elements.push_back(x);
  }

  ASSERT_EQ(elements.size(), expected.size());
  
  for (size_t i = 0; i < expected.size(); ++i)
  {
    ASSERT_EQ(elements[i], expected[i]);
  }
}

TEST(mymultiset, foreach_with_unordered_ints_duplicates)
{
  multiset<int> S;

  std::vector<int> expected = {21, 22, 20, 19, 23, 21, 21, 23};
  sort(expected.begin(), expected.end());

  for (int i : expected) {
    S.insert(i);
  }

  std::vector<int> elements; 
  
  for (int x : S)
  {
    elements.push_back(x);
  }

  ASSERT_EQ(S.size(), (int) expected.size());
  ASSERT_EQ(elements.size(), expected.size());
  
  for (size_t i = 0; i < expected.size(); ++i)
  {
    ASSERT_EQ(elements[i], expected[i]);
  }
}

TEST(mymultiset, foreach_with_single_string)
{
  multiset<string> S;
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

TEST(mymultiset, foreach_with_increasing_strings_no_duplicates)
{
  multiset<string> S;
  
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

TEST(mymultiset, count_with_unordered_strings_no_duplicates)
{
  multiset<string> S;

  std::vector<string> expected = {"fig", "apple", "cherry", "banana", "elderberry"};

  for (string s : expected) {
    S.insert(s);
  }
  
  for (string s : expected)
  {
    ASSERT_EQ(S.count(s), 1);
  }

  ASSERT_EQ(S.count("dragon fruit"), 0);
}

TEST(mymultiset, count_with_unordered_strings_duplicates)
{
  multiset<string> S;

  std::vector<string> expected = {"fig", "grape", "cherry", "banana", "elderberry"};

  for (string s : expected) {
    S.insert(s);
  }

  int dup1_count = 3;
  string dup1 = "apple";
  int dup2_count = 4;
  string dup2 = "honeydew";

  for (int i = 0; i < dup1_count; i++) {
    S.insert(dup1);
  }

  for (int i = 0; i < dup2_count; i++) {
    S.insert(dup2);
  }
  
  for (string s : expected)
  {
    ASSERT_EQ(S.count(s), 1);
  }
  ASSERT_EQ(S.count(dup1), dup1_count);
  ASSERT_EQ(S.count(dup2), dup2_count);

  ASSERT_EQ(S.count("dragon fruit"), 0);
}

TEST(mymultiset, count_with_movies_no_duplicates)
{
  multiset<Movie> S;

  Movie Sleepless("Sleepless in Seattle", 123, 123456789.00);
  Movie Matrix("The Matrix", 456, 400000000.00);
  Movie AnimalHouse("Animal House", 789, 1000000000.00);
  vector<Movie> expected = {Sleepless, Matrix, AnimalHouse};

  for (Movie m : expected) {
    S.insert(m);
  }
  
  for (Movie m : expected)
  {
    ASSERT_EQ(S.count(m), 1);
  }

  ASSERT_EQ(S.count(Movie("Goodfellas", 101, 0)), 0);
  ASSERT_EQ(S.count(Movie("The Matrix", 456, 0)), 1); // revenue doesn't matter for equality
  ASSERT_EQ(S.count(Movie("The Matrix", 789, 400000000.00)), 1); // ID doesn't matter for equality
  ASSERT_EQ(S.count(Movie("The Matrix", 0, 0)), 1); // neither revenue nor ID matters for equality
  ASSERT_EQ(S.count(Movie("the matrix", 456, 400000000.00)), 0); // case matter for equality
}

TEST(mymultiset, count_with_movies_duplicates)
{
  multiset<Movie> S;

  Movie Sleepless("Sleepless in Seattle", 123, 123456789.00);
  Movie Matrix("The Matrix", 456, 400000000.00);
  Movie AnimalHouse("Animal House", 789, 1000000000.00);
  vector<Movie> expected = {Sleepless, Matrix, AnimalHouse};

  int half_dup_count = 4;

  for (int i = 0; i < half_dup_count; i++) {
    S.insert(Movie("Napoleon Dynamite", i, i*100000));
  }
  
  for (Movie m : expected) {
    S.insert(m);
  }
  
  for (Movie m : expected)
  {
    ASSERT_EQ(S.count(m), 1);
  }

  for (int i = 0; i < half_dup_count; i++) {
    S.insert(Movie("Napoleon Dynamite", 2*i, 2*i*100000));
  }

  ASSERT_EQ(S.count(Movie("Napoleon Dynamite", 0, 0)), 2* half_dup_count);
}

// stress tests

const long long N_STRESS = 30;

TEST(mymultiset, foreach_stress_test_with_strictly_increasing_nums)
{
  multiset<int> S;
  for (int i = 0; i < N_STRESS; ++i) {
    S.insert(i);
  }

  // check multiset size
  ASSERT_EQ(S.size(), N_STRESS);

  // check values
  int count = 0;
  for (int x : S) {
    ASSERT_EQ(x, count);
    count++;
  }

  ASSERT_EQ(count, N_STRESS);
}

TEST(mymultiset, foreach_stress_test_with_random_nums)
{
  multiset<int> S;

  // populate a vector with random numbers
  // code below is from https://stackoverflow.com/a/23143753
  std::random_device rnd_device;
  std::mt19937 mersenne_engine {rnd_device()}; 
  std::uniform_int_distribution<int> dist {1, 42};
  auto gen = [&dist, &mersenne_engine](){ 
    return dist(mersenne_engine); 
  };
  vector<int> expected(N_STRESS);
  generate(begin(expected), end(expected), gen);

  // populate multiset
  for (int i = 0; i < N_STRESS; ++i) {
    S.insert(expected[i]);
  }

  std::sort(expected.begin(), expected.end());
  
  // code to remove duplicates (no longer applies)
  //
  // std::set<int> s;
  // unsigned size = sorted_expected.size();
  // for( unsigned i = 0; i < size; ++i ) s.insert( sorted_expected[i] );
  // sorted_expected.assign( s.begin(), s.end() );

  // check multiset size
  ASSERT_EQ(S.size(), (int) expected.size());

  // check values
  int count = 0;
  for (int x : S) {
    ASSERT_EQ(x, expected[count]);
    count++;
  }
}

TEST(mymultiset, toVector_with_duplicates)
{
  multiset<int> S;

  // 61 is repeated (3 instances)
  // 11 is repeated (2 instances)
  vector<int> V = { 22, 11, 49, 61, 3, 19, 35, 11, 61, 30, 41, 61 };

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

TEST(mymultiset, find_should_return_first_inserted_key)
{
  multiset<Movie> movies;

  movies.insert( Movie("The Matrix", 603, 1999) );
  movies.insert( Movie("Toy Story", 862, 1995) );
  movies.insert( Movie("Hamlet", 10, 1948) );
  movies.insert( Movie("Dracula", 5678, 1979) );
  movies.insert( Movie("Dracula", 1234, 1931) );
  movies.insert( Movie("Hamlet", 8, 2009) );
  movies.insert( Movie("Hamlet", 9, 1964) );

  auto iter = movies.find(Movie("Hercules", 0, 0));
  ASSERT_TRUE(iter == movies.end());

  iter = movies.find(Movie("Toy Story", 0, 0));
  ASSERT_TRUE((*iter).ID == 862);
  ASSERT_TRUE((*iter).Revenue == 1995);

  iter = movies.find(Movie("Dracula", 0, 0));
  ASSERT_TRUE((*iter).ID == 5678);
  ASSERT_TRUE((*iter).Revenue == 1979);

  iter = movies.find(Movie("Hamlet", 0, 0));
  ASSERT_TRUE((*iter).ID == 10);
  ASSERT_TRUE((*iter).Revenue == 1948);
}

TEST(mymultiset, erase_nonexistent_key)
{
  multiset<int> S;

  vector<int> before = {10, 15, 16, 11, 4, 13};
  vector<int> after = {10, 15, 16, 11, 4, 13};

  for (int x : before) {
    S.insert(x);
  }

  sort(before.begin(), before.end());

  ASSERT_EQ(S.size(), (int) before.size());
  int count = 0;
  for (int x : S) {
    ASSERT_EQ(x, before[count]);
    count++;
  }

  sort(after.begin(), after.end());

  S.erase(21);
  S.erase(-1);
  S.erase(0);
  S.erase(5);

  ASSERT_EQ(S.size(), (int) after.size());
  count = 0;
  for (int x : S) {
    ASSERT_EQ(x, after[count]);
    count++;
  }
}

TEST(mymultiset, erase_left_with_no_children)
{
  multiset<int> S;

  vector<int> before = {10, 9, 9, 8, 7, 7, 7, 5};
  vector<int> after = {10, 9, 9, 8, 7, 7, 7};

  for (int x : before) {
    S.insert(x);
  }

  sort(before.begin(), before.end());

  ASSERT_EQ(S.size(), (int) before.size());
  int count = 0;
  for (int x : S) {
    ASSERT_EQ(x, before[count]);
    count++;
  }

  sort(after.begin(), after.end());

  S.erase(5);

  ASSERT_EQ(S.size(), (int) after.size());
  count = 0;
  for (int x : S) {
    ASSERT_EQ(x, after[count]);
    count++;
  }
}

TEST(mymultiset, erase_left_with_no_children_duplicates)
{
  multiset<int> S;

  vector<int> before = {10, 9, 9, 8, 7, 7, 7, 5, 5, 5, 5};
  vector<int> after = {10, 9, 9, 8, 7, 7, 7};

  for (int x : before) {
    S.insert(x);
  }

  sort(before.begin(), before.end());

  ASSERT_EQ(S.size(), (int) before.size());
  int count = 0;
  for (int x : S) {
    ASSERT_EQ(x, before[count]);
    count++;
  }

  sort(after.begin(), after.end());

  S.erase(5);

  ASSERT_EQ(S.size(), (int) after.size());
  count = 0;
  for (int x : S) {
    ASSERT_EQ(x, after[count]);
    count++;
  }
}

TEST(mymultiset, erase_left_with_left_children_duplicates)
{
  multiset<int> S;

  vector<int> before = {10, 9, 9, 8, 7, 7, 7, 5, 5, 5, 5, 4, 3};
  vector<int> after = {10, 9, 9, 8, 7, 7, 7, 4, 3};

  for (int x : before) {
    S.insert(x);
  }

  sort(before.begin(), before.end());

  ASSERT_EQ(S.size(), (int) before.size());
  int count = 0;
  for (int x : S) {
    ASSERT_EQ(x, before[count]);
    count++;
  }

  sort(after.begin(), after.end());

  S.erase(5);

  ASSERT_EQ(S.size(), (int) after.size());
  count = 0;
  for (int x : S) {
    ASSERT_EQ(x, after[count]);
    count++;
  }
}

TEST(mymultiset, erase_right_with_no_children)
{
  multiset<int> S;

  vector<int> before = {10, 11, 12, 12, 12, 12, 14, 15};
  vector<int> after = {10, 11, 12, 12, 12, 12, 14};

  for (int x : before) {
    S.insert(x);
  }

  sort(before.begin(), before.end());

  ASSERT_EQ(S.size(), (int) before.size());
  int count = 0;
  for (int x : S) {
    ASSERT_EQ(x, before[count]);
    count++;
  }

  S.erase(15);

  ASSERT_EQ(S.size(), (int) after.size());
  count = 0;
  for (int x : S) {
    ASSERT_EQ(x, after[count]);
    count++;
  }
}

TEST(mymultiset, erase_right_with_no_children_duplicates)
{
  multiset<int> S;

  vector<int> before = {10, 11, 12, 12, 12, 12, 14, 15, 15, 15};
  vector<int> after = {10, 11, 12, 12, 12, 12, 14};

  for (int x : before) {
    S.insert(x);
  }

  sort(before.begin(), before.end());

  ASSERT_EQ(S.size(), (int) before.size());
  int count = 0;
  for (int x : S) {
    ASSERT_EQ(x, before[count]);
    count++;
  }

  S.erase(15);

  ASSERT_EQ(S.size(), (int) after.size());
  count = 0;
  for (int x : S) {
    ASSERT_EQ(x, after[count]);
    count++;
  }
}

TEST(mymultiset, erase_right_with_left_children_duplicates)
{
  multiset<int> S;

  vector<int> before = {10, 11, 12, 12, 12, 12, 15, 15, 15, 14};
  vector<int> after = {10, 11, 12, 12, 12, 12, 14};

  for (int x : before) {
    S.insert(x);
  }

  sort(before.begin(), before.end());

  ASSERT_EQ(S.size(), (int) before.size());
  int count = 0;
  for (int x : S) {
    ASSERT_EQ(x, before[count]);
    count++;
  }

  S.erase(15);

  ASSERT_EQ(S.size(), (int) after.size());
  count = 0;
  for (int x : S) {
    ASSERT_EQ(x, after[count]);
    count++;
  }
}

TEST(mymultiset, erase_right_with_right_children_duplicates)
{
  multiset<int> S;

  vector<int> before = {10, 11, 12, 12, 12, 12, 15, 15, 15, 16, 17, 18};
  vector<int> after = {10, 11, 12, 12, 12, 12, 16, 17, 18};
  int to_erase = 15;

  for (int x : before) {
    S.insert(x);
  }

  sort(before.begin(), before.end());

  ASSERT_EQ(S.size(), (int) before.size());
  int count = 0;
  for (int x : S) {
    ASSERT_EQ(x, before[count]);
    count++;
  }

  S.erase(to_erase);

  ASSERT_EQ(S.size(), (int) after.size());
  count = 0;
  for (int x : S) {
    ASSERT_EQ(x, after[count]);
    count++;
  }
}

TEST(mymultiset, erase_right_with_both_children_duplicates)
{
  multiset<int> S;

  vector<int> before = {1, 4, 5, 3, 2};
  vector<int> after = {1, 2, 3, 4};
  int to_erase = 4;

  for (int x : before) {
    S.insert(x);
  }

  sort(before.begin(), before.end());

  ASSERT_EQ(S.size(), (int) before.size());
  int count = 0;
  for (int x : S) {
    ASSERT_EQ(x, before[count]);
    count++;
  }

  S.erase(to_erase);

  ASSERT_EQ(S.size(), (int) after.size());
  count = 0;
  for (int x : S) {
    ASSERT_EQ(x, after[count]);
    count++;
  }
}

// instructor's tests 

TEST(mymultiset, empty_multiset)
{
  multiset<int> S;

  ASSERT_EQ(S.size(), 0);
}

TEST(mymultiset, multiset_with_one)
{
  multiset<int> S;

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

TEST(mymultiset, multiset_with_four_strings)
{
  multiset<string> S;

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

TEST(mymultiset, multiset_with_movies)
{
  multiset<Movie> S;

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

TEST(mymultiset, multiset_from_class_with_nine)
{
  multiset<int> S;

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

TEST(mymultiset, multiset_no_duplicates)
{
  multiset<int> S;

  vector<int> V = { 22, 11, 49, 3, 19, 35, 61, 30, 41 };

  for (auto x : V)
    S.insert(x);

  // try to insert them all again:
  for (auto x : V)
    S.insert(x);

  // should be twice the size since all numbers reinserted
  ASSERT_EQ(S.size(), 2 * (int) V.size());

  for (auto x : V) {
    ASSERT_TRUE(S.contains(x));
    ASSERT_TRUE(S[x]);
  }
}

TEST(mymultiset, toVector)
{
  multiset<int> S;

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

TEST(mymultiset, copy_empty)
{
  multiset<int> S1;

  {
    //
    // create a new scope, which will trigger destructor:
    //
    multiset<int> S2 = S1;  // this will call copy constructor:

    S1.insert(123);  // this should have no impact on S2:
    S1.insert(100);
    S1.insert(150);

    ASSERT_EQ(S2.size(), 0);

    vector<int> V2 = S2.toVector();

    ASSERT_EQ((int) V2.size(), 0);
  }
}

TEST(mymultiset, copy_constructor)
{
  multiset<int> S1;

  vector<int> V = { 22, 11, 49, 3, 19, 35, 61, 30, 41 };

  for (auto x : V)
    S1.insert(x);

  ASSERT_EQ(S1.size(), (int) V.size());

  {
    //
    // create a new scope, which will trigger destructor:
    //
    multiset<int> S2 = S1;  // this will call copy constructor:

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
  // the copy was just destroyed, the original multiset
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

TEST(mymultiset, find_empty)
{
  multiset<int> S;

  auto iter = S.find(22);
  ASSERT_TRUE(iter == S.end());
}

TEST(mymultiset, find_one)
{
  multiset<int> S;

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

TEST(mymultiset, find_with_multiset_from_class)
{
  multiset<int> S;

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

// instructor's stress test
// modified to use my N_STRESS instead of given N
// this makes quick testing easier

TEST(mymultiset, stress_test)
{
  multiset<long long> S;
  // edited to create a multiset, not a set
  std::multiset<long long> C;

  //
  // setup random number generator so tree will
  // be relatively balanced given insertion of
  // random numbers:
  //
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<long long> distrib(1, N_STRESS * 100);  // inclusive

  vector<long long> V;  // collect a few values for searching:
  int count = 0;

  while (S.size() != N_STRESS) {

    long long x = distrib(gen);

    S.insert(x);
    C.insert(x);

    count++;
    if (count == 1000) { // save every 1,000th value:

      V.push_back(x);
      count = 0;
    }
  }

  ASSERT_EQ(S.size(), N_STRESS);

  for (auto x : V) {
    ASSERT_TRUE(S.contains(x));
  }

  ASSERT_FALSE(S.contains(0));
  ASSERT_FALSE(S.contains(-1));

  //
  // now let's compare our multiset to C++ set:
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