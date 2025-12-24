#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <random>

#include "bstmap.h"

using namespace std;
using namespace testing;

namespace {

class Random {
 private:
  static mt19937 rng;

 public:
  static void seed(int s) {
    Random::rng.seed(s);
  }

  /**
   * Generate a random integer in the range 0 (inclusive) to `max` (exclusive)
   */
  static int randInt(int max) {
    return rng() % (max + 1);
  }
};

std::mt19937 Random::rng;


TEST(BSTMapCore, InsertAndSizegood) {
  BSTMap<int, string> bst;
  EXPECT_EQ(bst.size(), 0);
  EXPECT_TRUE(bst.empty());
  
  bst.insert(5, "five");
  EXPECT_EQ(bst.size(), 1);
  EXPECT_FALSE(bst.empty());
  
  bst.insert(3, "three");
  bst.insert(7, "seven");
  EXPECT_EQ(bst.size(), 3);
}

TEST(BSTMapCore, InsertDuplicateKeygood) {
  BSTMap<int, string> bst;
  bst.insert(5, "five");
  bst.insert(5, "new_five");
  
  EXPECT_EQ(bst.size(), 1);
  EXPECT_EQ(bst.at(5), "five");
}

TEST(BSTMapCore, Containsgood) {
  BSTMap<int, string> bst;
  bst.insert(5, "five");
  bst.insert(3, "three");
  bst.insert(7, "seven");
  
  EXPECT_TRUE(bst.contains(5));
  EXPECT_TRUE(bst.contains(3));
  EXPECT_TRUE(bst.contains(7));
  EXPECT_FALSE(bst.contains(2));
  EXPECT_FALSE(bst.contains(10));
}

TEST(BSTMapCore, Atgood) {
  BSTMap<int, string> bst;
  bst.insert(5, "five");
  bst.insert(3, "three");
  
  EXPECT_EQ(bst.at(5), "five");
  EXPECT_EQ(bst.at(3), "three");
  
  EXPECT_THROW(bst.at(10), out_of_range);
}

TEST(BSTMapCore, ToStringgood) {
  BSTMap<int, int> bst;
  bst.insert(3, 30);
  bst.insert(1, 10);
  bst.insert(4, 40);
  bst.insert(2, 20);

  string expected;
  expected = "1: 10\n";
  expected += "2: 20\n"; 
  expected += "3: 30\n";
  expected += "4: 40\n";
  
  EXPECT_EQ(bst.to_string(), expected);
}

TEST(BSTMapCore, Cleargood) {
  BSTMap<int, string> bst;
  bst.insert(5, "five");
  bst.insert(3, "three");
  bst.insert(7, "seven");
  
  EXPECT_EQ(bst.size(), 3);
  bst.clear();
  EXPECT_EQ(bst.size(), 0);
  EXPECT_TRUE(bst.empty());
  
  bst.insert(1, "one");
  EXPECT_EQ(bst.size(), 1);
  EXPECT_TRUE(bst.contains(1));
}

TEST(BSTMapCore, CopyConstructorgood) {
  BSTMap<int, string> original;
  original.insert(5, "five");
  original.insert(3, "three");
  original.insert(7, "seven");
  
  BSTMap<int, string> copy(original);
  
  EXPECT_EQ(original.size(), copy.size());
  EXPECT_EQ(original.at(5), copy.at(5));
  EXPECT_EQ(original.at(3), copy.at(3));
  EXPECT_EQ(original.at(7), copy.at(7));
  
  original.insert(10, "ten");
  EXPECT_FALSE(copy.contains(10));
}

TEST(BSTMapCore, AssignmentOperator) {
  BSTMap<int, string> original;
  original.insert(5, "five");
  original.insert(3, "three");
  BSTMap<int, string> copy;
  copy.insert(1, "one");
  copy = original;
  
  EXPECT_EQ(original.size(), copy.size());
  EXPECT_EQ(original.at(5), copy.at(5));
  EXPECT_EQ(original.at(3), copy.at(3));
  EXPECT_FALSE(copy.contains(1));
}

TEST(BSTMapCore, CopyConstructorEmpty) {
  BSTMap<int, string> empty_map;
  BSTMap<int, string> copy(empty_map);
  
  EXPECT_TRUE(copy.empty());
  EXPECT_EQ(copy.size(), 0);
}

TEST(BSTMapCore, AssignmentEmpty) {
  BSTMap<int, string> empty_map;
  BSTMap<int, string> target;
  target.insert(5, "five");  
  
  target = empty_map;  
  EXPECT_TRUE(target.empty());
  EXPECT_EQ(target.size(), 0);
}

TEST(BSTMapCore, SelfAssignment) {
  BSTMap<int, string> bst;
  bst.insert(5, "five");
  bst.insert(3, "three");
  
  BSTMap<int, string>& self_ref = bst;  
  bst = self_ref; 
  EXPECT_EQ(bst.size(), 2);
  EXPECT_TRUE(bst.contains(5));
  EXPECT_TRUE(bst.contains(3));
  EXPECT_EQ(bst.at(5), "five");
  EXPECT_EQ(bst.at(3), "three");
}

TEST(BSTMapCore, CopyConstructorDeepCopy) {
  BSTMap<int, string> original;
  original.insert(5, "five");
  original.insert(3, "three");
  original.insert(7, "seven");
  
  BSTMap<int, string> copy(original);
  original.insert(10, "ten");
  original.erase(3);
  
  EXPECT_EQ(copy.size(), 3);
  EXPECT_TRUE(copy.contains(3));
  EXPECT_TRUE(copy.contains(5));
  EXPECT_TRUE(copy.contains(7));
  EXPECT_FALSE(copy.contains(10));
}


TEST(BSTMapAugmented, RemoveMinEmpty) {
  BSTMap<int, string> bst;
  EXPECT_THROW(bst.remove_min(), runtime_error);
}

TEST(BSTMapAugmented, RemoveMinSingle) {
  BSTMap<int, string> bst;
  bst.insert(5, "five");
  
  auto result = bst.remove_min();
  EXPECT_EQ(result.first, 5);
  EXPECT_EQ(result.second, "five");
  EXPECT_TRUE(bst.empty());
  EXPECT_EQ(bst.size(), 0);
}

TEST(BSTMapAugmented, RemoveMinMultiple) {
  BSTMap<int, string> bst;
  bst.insert(5, "five");
  bst.insert(3, "three");
  bst.insert(7, "seven");
  bst.insert(1, "one");
  
  auto result1 = bst.remove_min();
  EXPECT_EQ(result1.first, 1);
  EXPECT_EQ(result1.second, "one");
  EXPECT_EQ(bst.size(), 3);
  EXPECT_FALSE(bst.contains(1));
  
  auto result2 = bst.remove_min();
  EXPECT_EQ(result2.first, 3);
  EXPECT_EQ(result2.second, "three");
  EXPECT_EQ(bst.size(), 2);
}

TEST(BSTMapAugmented, RemoveMinTreeStructure) {
  BSTMap<int, string> bst;
  bst.insert(5, "five");
  bst.insert(3, "three");
  bst.insert(7, "seven");
  bst.insert(2, "two");
  bst.insert(4, "four");
  
  auto result = bst.remove_min();
  EXPECT_EQ(result.first, 2);
  EXPECT_EQ(result.second, "two");
  EXPECT_EQ(bst.size(), 4);
  
  EXPECT_TRUE(bst.contains(3));
  EXPECT_TRUE(bst.contains(4));
  EXPECT_TRUE(bst.contains(5));
  EXPECT_TRUE(bst.contains(7));
  EXPECT_EQ(bst.at(3), "three");
}

TEST(BSTMapAugmented, OperatorEqualsBasic) {
  BSTMap<int, string> map1;
  BSTMap<int, string> map2;
  
  map1.insert(1, "one");
  map1.insert(2, "two");
  map2.insert(1, "one");
  map2.insert(2, "two");
  
  EXPECT_TRUE(map1 == map2);
}

TEST(BSTMapAugmented, OperatorEqualsDifferentValues) {
  BSTMap<int, string> map1;
  BSTMap<int, string> map2;
  
  map1.insert(1, "one");
  map1.insert(2, "two");
  map2.insert(1, "one");
  map2.insert(2, "different");
  EXPECT_FALSE(map1 == map2);
}

TEST(BSTMapAugmented, OperatorEqualsDifferentSizes) {
  BSTMap<int, string> map1;
  BSTMap<int, string> map2;
  
  map1.insert(1, "one");
  map1.insert(2, "two");
  map2.insert(1, "one");
  EXPECT_FALSE(map1 == map2);
}

TEST(BSTMapAugmented, OperatorEqualsEmpty) {
  BSTMap<int, string> map1;
  BSTMap<int, string> map2;
  EXPECT_TRUE(map1 == map2);
}

TEST(BSTMapAugmented, BeginEmpty) {
  BSTMap<int, string> bst;
  bst.begin();
  int key;
  string val;
  EXPECT_FALSE(bst.next(key, val));
}

TEST(BSTMapAugmented, BeginNextBasic) {
  BSTMap<int, string> bst;
  bst.insert(5, "five");
  bst.insert(3, "three");
  bst.insert(7, "seven");
  bst.insert(1, "one");
  bst.insert(4, "four");
  
  bst.begin();
  int key;
  string  val;
  

  EXPECT_TRUE(bst.next(key, val));
  EXPECT_EQ(key, 1);
  EXPECT_EQ(val, "one");
  
  EXPECT_TRUE(bst.next(key, val));
  EXPECT_EQ(key, 3);
  EXPECT_EQ(val, "three");
  
  EXPECT_TRUE(bst.next(key, val));
  EXPECT_EQ(key, 4);
  EXPECT_EQ(val, "four");
  
  EXPECT_TRUE(bst.next(key, val));
  EXPECT_EQ(key, 5);
  EXPECT_EQ(val, "five");
  
  EXPECT_TRUE(bst.next(key, val));
  EXPECT_EQ(key, 7);
  EXPECT_EQ(val, "seven");
  
  EXPECT_FALSE(bst.next(key, val));
}

TEST(BSTMapAugmented, BeginNextSingle) {
  BSTMap<int, string> bst;
  bst.insert(5, "five");
  
  bst.begin();
  int key;
  string val;
  
  EXPECT_TRUE(bst.next(key, val));
  EXPECT_EQ(key, 5);
  EXPECT_EQ(val, "five");
  
  EXPECT_FALSE(bst.next(key, val));
}

TEST(BSTMapAugmented, NextAfterEnd) {
  BSTMap<int, string> bst;
  bst.insert(1, "one");
  
  bst.begin();
  int key;
  string val;
  
  bst.next(key, val);  
  EXPECT_FALSE(bst.next(key, val));  
  EXPECT_FALSE(bst.next(key, val));  
}

TEST(BSTMapAugmented, BeginNextAllElements) {
  BSTMap<int, int> bst;
  vector<int> keys = {5, 3, 7, 1, 4, 6, 8};
  
  for (int key : keys) {
    bst.insert(key, key * 10);
  }
  
  bst.begin();
  int key;
  int val;
  vector<int> traversed;
  
  while (bst.next(key, val)) {
    traversed.push_back(key);
  }
  

  EXPECT_EQ(traversed.size(), 7);
  for (size_t i = 1; i < traversed.size(); i++) {
    EXPECT_LT(traversed[i-1], traversed[i]);
  }
}
TEST(BSTMapErase, EraseNotFoundgood) {
  BSTMap<int, string> bst;
  bst.insert(5, "five");
  
  EXPECT_THROW(bst.erase(10), out_of_range);
  EXPECT_EQ(bst.size(), 1);
}

TEST(BSTMapErase, EraseLeafgood) {
  BSTMap<int, string> bst;
  bst.insert(5, "five");
  bst.insert(3, "three");
  bst.insert(7, "seven");
  
  string result = bst.erase(3);
  EXPECT_EQ(result, "three");
  EXPECT_EQ(bst.size(), 2);
  EXPECT_FALSE(bst.contains(3));
  EXPECT_TRUE(bst.contains(5));
  EXPECT_TRUE(bst.contains(7));
}

TEST(BSTMapErase, EraseOneChildgood) {
  BSTMap<int, string> bst;
  bst.insert(5, "five");
  bst.insert(3, "three");
  bst.insert(2, "two");
  
  string result = bst.erase(3);
  EXPECT_EQ(result, "three");
  EXPECT_EQ(bst.size(), 2);
  EXPECT_FALSE(bst.contains(3));
  EXPECT_TRUE(bst.contains(2));
  EXPECT_TRUE(bst.contains(5));
}

TEST(BSTMapErase, EraseTwoChildrengood) {
  BSTMap<int, string> bst;
  bst.insert(5, "five");
  bst.insert(3, "three");
  bst.insert(7, "seven");
  bst.insert(6, "six");
  bst.insert(8, "eight");
  
  string result = bst.erase(7);
  EXPECT_EQ(result, "seven");
  EXPECT_EQ(bst.size(), 4);
  EXPECT_FALSE(bst.contains(7));
  EXPECT_TRUE(bst.contains(6));
  EXPECT_TRUE(bst.contains(8));
  EXPECT_TRUE(bst.contains(5));
  EXPECT_TRUE(bst.contains(3));
}

TEST(BSTMapErase, EraseRootAlonegood) {
  BSTMap<int, string> bst;
  bst.insert(5, "five");
  
  string result = bst.erase(5);
  EXPECT_EQ(result, "five");
  EXPECT_TRUE(bst.empty());
  EXPECT_EQ(bst.size(), 0);
}

TEST(BSTMapErase, EraseRootWithChildrengood) {
  BSTMap<int, string> bst;
  bst.insert(5, "five");
  bst.insert(3, "three");
  bst.insert(7, "seven");
  
  string result = bst.erase(5);
  EXPECT_EQ(result, "five");
  EXPECT_EQ(bst.size(), 2);
  EXPECT_FALSE(bst.contains(5));
  EXPECT_TRUE(bst.contains(3));
  EXPECT_TRUE(bst.contains(7));
}

TEST(BSTMapErase, EraseMultiplegood) {
  BSTMap<int, string> bst;
  bst.insert(5, "five");
  bst.insert(3, "three");
  bst.insert(7, "seven");
  bst.insert(1, "one");
  bst.insert(4, "four");
  bst.insert(6, "six");
  bst.insert(8, "eight");
  
  bst.erase(3);
  bst.erase(7);
  bst.erase(1);
  
  EXPECT_EQ(bst.size(), 4);
  EXPECT_FALSE(bst.contains(3));
  EXPECT_FALSE(bst.contains(7));
  EXPECT_FALSE(bst.contains(1));
  EXPECT_TRUE(bst.contains(5));
  EXPECT_TRUE(bst.contains(4));
  EXPECT_TRUE(bst.contains(6));
  EXPECT_TRUE(bst.contains(8));
}

TEST(BSTMapAugmented, OperatorEquals3) {
  BSTMap<int, string> map1;
  BSTMap<int, string> map2;

  map1.insert(5, "five");
  map1.insert(3, "three");
  map1.insert(7, "seven");
  map1.insert(1, "one");
  map1.insert(4, "four");
  map1.insert(6, "six");
  map1.insert(8, "eight");
  

  map2.insert(1, "one");
  map2.insert(3, "three");
  map2.insert(4, "four");
  map2.insert(5, "five");
  map2.insert(6, "six");
  map2.insert(7, "seven");
  map2.insert(8, "eight");
  
  EXPECT_TRUE(map1 == map2);
}

TEST(BSTMapAugmented, OperatorEquals1) {
  BSTMap<int, string> map1;
  BSTMap<int, string> map2;
  

  map1.insert(1, "one");
  map1.insert(2, "two");
  map1.insert(3, "three");
  
  map2.insert(1, "one");
  map2.insert(2, "two");
  
  EXPECT_FALSE(map1 == map2);
}

TEST(BSTMapAugmented, OperatorEquals2) {
  BSTMap<int, string> map1;
  BSTMap<int, string> map2;

  map1.insert(2, "two");
  map1.insert(2, "two"); 
  
  map2.insert(1, "one");
  map2.insert(2, "two");
  
  EXPECT_FALSE(map1 == map2);
}

TEST(BSTMapErase, SuccessorChildren1) {
    BSTMap<int, string> bst;
    bst.insert(10, "ten");
    bst.insert(5, "five");
    bst.insert(15, "fifteen");
    bst.insert(13, "thirteen");
    bst.insert(20, "twenty");
    bst.insert(12, "twelve");
    bst.insert(14, "fourteen");
    
    EXPECT_EQ(bst.size(), 7);
    
    string result = bst.erase(15);
    EXPECT_EQ(result, "fifteen");
    EXPECT_EQ(bst.size(), 6);
    
    EXPECT_FALSE(bst.contains(15));
    EXPECT_TRUE(bst.contains(10));
    EXPECT_TRUE(bst.contains(5));
    EXPECT_TRUE(bst.contains(13));
    EXPECT_TRUE(bst.contains(20));
    EXPECT_TRUE(bst.contains(12));
    EXPECT_TRUE(bst.contains(14));
    
    bst.begin();
    int key;
    string val;
    int count = 0;
    int last_key = -1;
    while (bst.next(key, val)) {
        count++;
        EXPECT_GT(key, last_key);
        last_key = key;
    }
    EXPECT_EQ(count, 6);
}

TEST(BSTMapErase, SuccessorChild2) {
    BSTMap<int, string> bst;
    bst.insert(10, "ten");
    bst.insert(5, "five");
    bst.insert(20, "twenty");
    bst.insert(15, "fifteen");
    bst.insert(25, "twenty-five");
    bst.insert(13, "thirteen");
    bst.insert(14, "fourteen");
    
    EXPECT_EQ(bst.size(), 7);
    
    string result = bst.erase(10);
    EXPECT_EQ(result, "ten");
    EXPECT_EQ(bst.size(), 6);
    
    EXPECT_FALSE(bst.contains(10));
    EXPECT_TRUE(bst.contains(5));
    EXPECT_TRUE(bst.contains(20));
    EXPECT_TRUE(bst.contains(15));
    EXPECT_TRUE(bst.contains(25));
    EXPECT_TRUE(bst.contains(13));
    EXPECT_TRUE(bst.contains(14));
    
    bst.begin();
    int key;
    string val;
    int count = 0;
    int last_key = -1;
    while (bst.next(key, val)) {
        count++;
        EXPECT_GT(key, last_key);
        last_key = key;
    }
    EXPECT_EQ(count, 6);
}

TEST(BSTMapErase, SuccessorChild3) {
    BSTMap<int, string> bst;
    bst.insert(10, "ten");
    bst.insert(5, "five");
    bst.insert(15, "fifteen");
    bst.insert(13, "thirteen");
    bst.insert(20, "twenty");
    bst.insert(12, "twelve");
    bst.insert(14, "fourteen");
    bst.insert(16, "sixteen");
    
    EXPECT_EQ(bst.size(), 8);
    
    string result = bst.erase(15);
    EXPECT_EQ(result, "fifteen");
    EXPECT_EQ(bst.size(), 7);
    
    EXPECT_FALSE(bst.contains(15));
    EXPECT_TRUE(bst.contains(14));
    EXPECT_TRUE(bst.contains(16));
    
    bst.begin();
    int key;
    string val;
    int count = 0;
    int last_key = -1;
    while (bst.next(key, val)) {
        count++;
        EXPECT_GT(key, last_key);
        last_key = key;
    }
    EXPECT_EQ(count, 7);
}
} // namespace
