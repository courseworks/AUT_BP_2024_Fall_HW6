#include <gtest/gtest.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

// Since we're testing C code, we need to prevent C++ name mangling
extern "C" {
#include "Q1_Trie.h"
}

// Test Fixture for Trie
class TrieTest : public ::testing::Test {
   protected:
	Trie* trie;

	// Called before each test
	void SetUp() override { trie = createTrie(); }

	// Called after each test
	void TearDown() override { freeTrie(trie); }
};

// Test 1: Trie Creation
TEST_F(TrieTest, CreateTrie) {
	ASSERT_NE(trie, nullptr) << "Failed to create Trie.";
	ASSERT_NE(trie->root, nullptr)
		<< "Trie root should not be NULL. After calling createTrie() function.";
}

// Test 2: Insertion and Search
TEST_F(TrieTest, InsertAndSearchWords) {
	const char* words[] = {"apple", "app", "application", "bat", "batch"};
	size_t num_words = sizeof(words) / sizeof(words[0]);

	// Insert words
	for (size_t i = 0; i < num_words; ++i) {
		insertWord(trie, words[i]);
	}

	// Search for inserted words
	for (size_t i = 0; i < num_words; ++i) {
		EXPECT_TRUE(searchWord(trie, words[i]))
			<< "Word '" << words[i] << "' should be found in Trie.";
	}

	// Search for non-inserted words
	const char* non_words[] = {"baton", "banana", "appl", "batt",
							   "applicationa"};
	size_t num_non_words = sizeof(non_words) / sizeof(non_words[0]);

	for (size_t i = 0; i < num_non_words; ++i) {
		EXPECT_FALSE(searchWord(trie, non_words[i]))
			<< "Word '" << non_words[i] << "' should not be found in Trie.";
	}
}

// Test 3: Deletion
TEST_F(TrieTest, DeleteWords) {
	const char* words[] = {"apple", "app", "application", "bat", "batch"};
	size_t num_words = sizeof(words) / sizeof(words[0]);

	// Insert words
	for (size_t i = 0; i < num_words; ++i) {
		insertWord(trie, words[i]);
	}

	// Delete a word that is a prefix of another word
	deleteWord(trie, "apple");
	EXPECT_FALSE(searchWord(trie, "apple"))
		<< "Word 'apple' should have been deleted.";
	EXPECT_TRUE(searchWord(trie, "app")) << "Word 'app' should still exist.";
	EXPECT_TRUE(searchWord(trie, "application"))
		<< "Word 'application' should still exist.";

	// Delete a word that is not a prefix
	deleteWord(trie, "bat");
	EXPECT_FALSE(searchWord(trie, "bat"))
		<< "Word 'bat' should have been deleted.";
	EXPECT_TRUE(searchWord(trie, "batch"))
		<< "Word 'batch' should still exist.";

	// Attempt to delete a word that does not exist
	deleteWord(trie, "banana");
	// No crash should occur, and existing words remain unaffected
	EXPECT_TRUE(searchWord(trie, "batch"))
		<< "Word 'batch' should still exist after attempting to delete "
		   "'banana'.";
}

// Test 4: Save and Load Trie
TEST_F(TrieTest, SaveAndLoadTrie) {
	const char* words[] = {"apple", "app", "application", "bat", "batch"};
	size_t num_words = sizeof(words) / sizeof(words[0]);

	// Insert words
	for (size_t i = 0; i < num_words; ++i) {
		insertWord(trie, words[i]);
	}

	// Save Trie to a temporary file
	const char* temp_filename = "temp_trie_save.txt";
	saveTrie(trie, temp_filename);

	// Load Trie from the file
	Trie* loaded_trie = loadTrie(temp_filename);
	ASSERT_NE(loaded_trie, nullptr) << "Failed to load Trie from file.";

	// Verify all words are present in the loaded trie
	for (size_t i = 0; i < num_words; ++i) {
		EXPECT_TRUE(searchWord(loaded_trie, words[i]))
			<< "Word '" << words[i] << "' should be found in loaded Trie.";
	}

	// Verify that non-inserted words are not present
	const char* non_words[] = {"baton", "banana", "appl", "batt",
							   "applicationa"};
	size_t num_non_words = sizeof(non_words) / sizeof(non_words[0]);

	for (size_t i = 0; i < num_non_words; ++i) {
		EXPECT_FALSE(searchWord(loaded_trie, non_words[i]))
			<< "Word '" << non_words[i]
			<< "' should not be found in loaded Trie.";
	}

	// Clean up
	freeTrie(loaded_trie);
	// Remove the temporary file
	std::remove(temp_filename);
}

// Test 5: Edge Cases
TEST_F(TrieTest, EdgeCases) {
	// Insert words with uppercase letters
	insertWord(trie, "Apple");
	insertWord(trie, "BaTCh");

	EXPECT_TRUE(searchWord(trie, "apple"))
		<< "Word 'apple' should be found (case-insensitive).";
	EXPECT_TRUE(searchWord(trie, "batch"))
		<< "Word 'batch' should be found (case-insensitive).";

	// Insert duplicate words
	insertWord(trie, "apple");
	EXPECT_TRUE(searchWord(trie, "apple"))
		<< "Word 'apple' should still be found after duplicate insertion.";

	// Delete word that doesn't exist
	deleteWord(trie, "nonexistent");
	// Ensure existing words are unaffected
	EXPECT_TRUE(searchWord(trie, "apple"))
		<< "Word 'apple' should still exist after attempting to delete a "
		   "nonexistent word.";
}

// Test 6: Empty Trie Operations
TEST_F(TrieTest, EmptyTrie) {
	// Search in empty trie
	EXPECT_FALSE(searchWord(trie, "anyword"))
		<< "Searching in an empty Trie should return false.";

	// Delete in empty trie (should not crash)
	deleteWord(trie, "anyword");
	// Still empty
	EXPECT_FALSE(searchWord(trie, "anyword"))
		<< "After deleting in an empty Trie, search should still return false.";
}

// Test 7: Long Words
TEST_F(TrieTest, LongWords) {
	// Assuming the maximum word length is less than 256 as per implementation
	char long_word[256];
	memset(long_word, 'a', 255);
	long_word[255] = '\0';

	insertWord(trie, long_word);
	EXPECT_TRUE(searchWord(trie, long_word))
		<< "Long word should be inserted and found successfully.";

	char too_long_word[300];
	memset(too_long_word, 'b', 299);
	too_long_word[299] = '\0';

	insertWord(trie, too_long_word);
	EXPECT_TRUE(searchWord(trie, too_long_word))
		<< "Too long word should be inserted and found successfully (behavior "
		   "depends on implementation).";
}

// Test 8: Special Characters in Insertions
TEST_F(TrieTest, SpecialCharacters) {
	// Insert words with various special characters
	insertWord(trie, "hello-world");
	insertWord(trie, "good_morning");
	insertWord(trie, "c++");

	// Expected behavior: non-alphabetic characters are ignored, so
	// "helloworld", "goodmorning", "c" should be inserted
	EXPECT_TRUE(searchWord(trie, "helloworld"))
		<< "Word 'helloworld' should be found after inserting 'hello-world'.";
	EXPECT_TRUE(searchWord(trie, "goodmorning"))
		<< "Word 'goodmorning' should be found after inserting 'good_morning'.";
	EXPECT_TRUE(searchWord(trie, "c"))
		<< "Word 'c' should be found after inserting 'c++'.";
}
