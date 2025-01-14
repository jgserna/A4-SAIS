#include "SAIS.h"
#include"DC3.h"
#include <iostream>
#include <cstddef>
#include <unordered_set>
using namespace std;

vector<size_t> inducedSort(const vector<size_t>& text, const vector<size_t>& LMSindices, const vector<string>& LMS, vector<vector<int>> heads_tails, const vector<vector<int>>& heads_tails2);
vector<vector<int>> resetIndices(vector<vector<int>> heads_tails, const vector<vector<int>>& heads_tails2);

bool compare(const vector<size_t>& text, size_t begin1, size_t end1, size_t begin2, size_t end2);

SuffixArray sais(const vector<size_t>& text) {
bool allDistinct(const vector<size_t>& vec);

/*
//	display text
	cout << "text: ";
	for (size_t i = 0; i < text.size(); i++){
		cout << text[i] << " ";
	}
	cout << endl;

*/

//Create LMS array and tag with l or s================================
	vector<string> LMS;
	LMS.resize(text.size());

	LMS[text.size() - 1] = "s";

	for(size_t i = text.size() - 2; i < text.size(); i--){
		if(text[i] < text[i + 1]){
			LMS[i] = "s";
		}
		if(text[i] > text[i + 1]){
			LMS[i] = "l";
		}
		if(text[i] == text[i + 1]){
			LMS[i] = LMS[i + 1];
		}
		if(i == 0){
		break;
		}
	}
/*
	cout << "LMS:  ";
	for(size_t i = 0; i < LMS.size(); i++){
		cout <<  LMS[i] << "|";
	}
	cout << endl;
//====================================================================
*/


//Create LMSIndices array, locate and save the indices of the LMS=====
	vector<size_t> LMSindices;

	for(size_t i = 1; i < LMS.size(); i++){
		if(LMS[i - 1] == "l" && LMS[i] == "s") {
			LMSindices.push_back(i);
		}
	}

/*/test print
	cout << "lms indices: ";
	for(size_t i = 0; i < LMSindices.size(); i++){
		cout <<  LMSindices[i] << "|";
	}
	cout << endl;

//=====================================================================
*/

	size_t num_buckets = 0;
	size_t table[27] = {0};

//find and count distinct elements
	for(size_t i = 0; i< text.size(); i++){
		table[text[i]] = table[text[i]] + 1;
	}


	for(size_t i = 0; i < text.size(); i++) {
		if(table[i] > 0) {
			num_buckets++;
			//cout << "buckets table " << i << ": " << table[i] << endl;
			}
	}
	cout << endl;


//====================================================================
/*
	int **heads_tails = new int *[num_buckets];
	int **heads_tails2 = new int *[num_buckets];


	for (size_t i = 0; i < num_buckets; i++){
		heads_tails[i] = new int[2];
		heads_tails2[i] = new int[2];
	}

	int count = 0;
	for(size_t i = 0; i < num_buckets; i++){
		int left = count;
		int right = count + table[i] - 1;
		heads_tails[i][0] = left;
		heads_tails[i][1] = right;
		count += table[i];
	}

	cout << "hts1: " << endl;
	for (size_t i = 0; i < num_buckets; i++){
		for (int j = 0; j < 2; j++){
			cout << heads_tails[i][j] << "|";
		}
	cout << endl;

	}
	for (size_t i = 0; i < num_buckets; i++){
		for (int j = 0; j < 2; j++){
			heads_tails2[i][j] = heads_tails[i][j];
		}
	cout << endl;
	}

	cout <<"hts2: " << endl;
	for (size_t i = 0; i < num_buckets; i++){
		for (int j = 0; j < 2; j++){
			cout << heads_tails2[i][j] << "|";
		}
	cout << endl;
	}

*/
//================alt heads tails===========================

	vector<vector<int>> heads_tails(num_buckets, vector<int>(2));
	int count = 0;

	for(size_t i = 0; i < num_buckets; i++){
		int left = count;
		int right = count + table[i] - 1;
		heads_tails[i][0] = left;
		heads_tails[i][1] = right;
		count += table[i];
	}

	vector<vector<int>> heads_tails2(heads_tails);
/*
	cout << "hts1: " << endl;
	for (size_t i = 0; i < num_buckets; i++){
		for (int j = 0; j < 2; j++){
			cout << heads_tails[i][j] << "|";
		}
	cout << endl;
	}

	cout <<"hts2: " << endl;
	for (size_t i = 0; i < num_buckets; i++){
		for (int j = 0; j < 2; j++){
			cout << heads_tails2[i][j] << "|";
		}
	cout << endl;
	}
*/

//=================calling step 2 function


	vector<size_t> textSorted;

	for(size_t i = 0; i < text.size(); i++){
			textSorted.push_back(0);
	}

	textSorted = inducedSort(text, LMSindices, LMS, heads_tails, heads_tails2);


/*
//====================1st pass=======insert LMS=================

	vector<size_t> textSorted;
//	int textSize = text.size();

	for(size_t i = 0; i < text.size(); i++){
			textSorted.push_back(0);
	}


	for(int i = LMSindices.size() - 1; i >= 0; i--) {

		int letter = text[LMSindices[i]];
	 	if (heads_tails[letter][0] == heads_tails[letter][1]) {
			textSorted[heads_tails[letter][0]] = LMSindices[i];

		} else {
			textSorted[heads_tails[letter][1]] = LMSindices[i];
			heads_tails[letter][1] = heads_tails[letter][1] - 1;

		}
	}
	cout << "after first pass: " << endl;
	for (size_t i = 0; i < text.size(); i++){
		cout << textSorted[i] << "|";
	}
	cout << endl;

//===============2nd pass========================================


	for (size_t i = 0; i < text.size(); i++) {
		if (textSorted[i] != 0) {
			size_t j  = textSorted[i] - 1;
			size_t letter = text[j];
			if (LMS[j] == "l") {
				if (heads_tails[letter][0] == heads_tails[letter][1]) {
					textSorted[heads_tails[letter][0]]  = j;
				} else {
					textSorted[heads_tails[letter][0]] = j;
					heads_tails[letter][0] = heads_tails[letter][0] + 1;
				}
			}
		}
	}


	cout << "after second pass: " << endl;
	for (size_t i = 0; i < text.size(); i++){
		cout << textSorted[i] << "|";
	}
	cout << endl;

//==============3rd pass==========================================


	for (int i = text.size() - 1; i > 0; i--)
	{
		if(textSorted[i] != 0)
		{
			int j = textSorted[i] - 1;
			int letter = text[j];
			if(LMS[j] == "s")
			{
				if (heads_tails2[letter][0] == heads_tails2[letter][1])
				{
					textSorted[heads_tails2[letter][0]] = j;
					}else{
					textSorted[heads_tails2[letter][1]] = j;
					heads_tails2[letter][1] = heads_tails2[letter][1] - 1;
				}
			}
		}
	}

	cout << "after third pass: " << endl;
	for (size_t i = 0; i < text.size(); i++){
		cout << textSorted[i] << "|";
	}
	cout << endl;

*/

//==================reduced text=================================


	vector<size_t> lmsTails;
	for (size_t i = 0; i < text.size(); i++){
		lmsTails.push_back(0);
	}

	for (size_t i = 0; i < LMSindices.size(); i++){
		lmsTails[LMSindices[i]] = LMSindices[i + 1];
	}
	lmsTails.back() = LMSindices.back(); //the last is always $ thus always lms

//	cout << "lmsTails" << endl;
//	for (size_t i = 0; i < lmsTails.size(); i++) {
//		cout << lmsTails[i] << "|";
//	}
//	cout << endl;

	vector<size_t> lmsTracker;
	vector<int> reducedText;
	for (size_t i = 0; i < text.size(); i++){
		reducedText.push_back(-1);
	}

	count = 0;
	size_t lmsCount = 0;

	lmsTracker.push_back(textSorted.front());
	reducedText[textSorted.front()] = count;
	count++;

	for (size_t i = 1; i < text.size(); i++){
		if (LMS[textSorted[i]] == "s" && LMS[textSorted[i] - 1] == "l"){
			lmsTracker.push_back(textSorted[i]);
			lmsCount++;

			size_t begin1 = textSorted[i];
			size_t end1 = lmsTails[textSorted[i]];
			size_t begin2 = lmsTracker[lmsCount - 1];
			size_t end2 = lmsTails[begin2];

			bool result = compare(text, begin1, end1, begin2, end2);

			if (result) {
				count--;
				reducedText[textSorted[i]] = count;
			}else{
				reducedText[textSorted[i]] = count;
				count++;
			}
		}
	}
/*
	cout << "reduced text: " << endl;
	for (size_t i = 0; i < reducedText.size(); i++){
		if(reducedText[i] == -1){
			cout << " |";
		}else{
			cout << reducedText[i] << "|";
		}
	}
	cout << endl;
*/

//==============================step 4====================================

	vector<size_t> newText(reducedText.size());
	size_t index = 0;
	for (size_t i = 0; i < reducedText.size(); i++){
		if (reducedText[i] != -1){
			newText[index] = reducedText[i];
			index++;
		}
	}
	newText.resize(index);

//	vector<size_t> dc3(newText.size());

	const auto dc3Text = dc3(newText);

//	if (!allDistinct(newText)){
//		dc3Text = dc3(newText);
//	}else{
//		dc3Text(newText);
//	}

//insert an checkpoint if numbers in new text are all distinct, then no recursion?
	//const auto dc3Text = sais(newText);
/*
	cout << "after dc3: " << endl;
	for (size_t i = 0; i < dc3Text.size(); i++){
		cout << dc3Text[i] << "|";
	}
	cout << endl;
*/
	vector<size_t> lmsSuffixes(LMSindices.size());
	for (size_t i = 0; i < LMSindices.size(); i++){
		lmsSuffixes[i] = LMSindices[dc3Text[i]];
	}
/*
	cout << "lms suffixes: " << endl;
	for (size_t i = 0; i < lmsSuffixes.size(); i++){
		cout << lmsSuffixes[i] << "|";
	}
	cout << endl;
*/

	textSorted = inducedSort(text, lmsSuffixes, LMS, heads_tails, heads_tails2);


  return textSorted;

}
//=============================================================================================================================
//                                 HELPER FUNCTIONS
//=============================================================================================================================

bool compare(const vector<size_t>& text, size_t begin1, size_t end1, size_t begin2, size_t end2){
	if(end1 - begin1 != end2 - begin2){
		return false;
	}

	for (size_t i = begin1, j = begin2; i <= end1 && j <= end2; i++, j++) {

		if (text[i] != text[j]) {
			return false;
		}
	}
	return true;
}



vector<vector<int>> resetIndices(vector<vector<int>> heads_tails, const vector<vector<int>>& heads_tails2) {
	for(size_t i = 0; i < heads_tails2.size(); i++){
		heads_tails[i][0] = heads_tails2[i][0];
		heads_tails[i][1] = heads_tails2[i][1];
	}
	return heads_tails;
}



vector<size_t> inducedSort(const vector<size_t>& text, const vector<size_t>& LMSindices, const vector<string>& LMS, vector<vector<int>> heads_tails, const vector<vector<int>>& heads_tails2) {


//====================1st pass=======insert LMS=================

	vector<size_t> textSorted;
//	int textSize = text.size();

	for(size_t i = 0; i < text.size(); i++){
			textSorted.push_back(0);
	}

	heads_tails = resetIndices(heads_tails, heads_tails2);

	for(int i = LMSindices.size() - 1; i >= 0; i--) {

		int letter = text[LMSindices[i]];
	 	if (heads_tails[letter][0] == heads_tails[letter][1]) {
			textSorted[heads_tails[letter][0]] = LMSindices[i];

		} else {
			textSorted[heads_tails[letter][1]] = LMSindices[i];
			heads_tails[letter][1] = heads_tails[letter][1] - 1;

		}
	}

	heads_tails = resetIndices(heads_tails, heads_tails2);

/*
	cout << "after first pass: " << endl;
	for (size_t i = 0; i < text.size(); i++){
		cout << textSorted[i] << "|";
	}
	cout << endl;
*/
//===============2nd pass========================================


	for (size_t i = 0; i < text.size(); i++) {
		if (textSorted[i] != 0) {
			size_t j  = textSorted[i] - 1;
			size_t letter = text[j];
			if (LMS[j] == "l") {
				if (heads_tails[letter][0] == heads_tails[letter][1]) {
					textSorted[heads_tails[letter][0]]  = j;
				} else {
					textSorted[heads_tails[letter][0]] = j;
					heads_tails[letter][0] = heads_tails[letter][0] + 1;
				}
			}
		}
	}


	heads_tails = resetIndices(heads_tails, heads_tails2);
/*
	cout << "after second pass: " << endl;
	for (size_t i = 0; i < text.size(); i++){
		cout << textSorted[i] << "|";
	}
	cout << endl;
*/
//==============3rd pass==========================================


	for (int i = text.size() - 1; i > 0; i--)
	{
		if(textSorted[i] != 0)
		{
			int j = textSorted[i] - 1;
			int letter = text[j];
			if(LMS[j] == "s")
			{
				if (heads_tails[letter][0] == heads_tails[letter][1])
				{
					textSorted[heads_tails[letter][0]] = j;
					}else{
					textSorted[heads_tails[letter][1]] = j;
					heads_tails[letter][1] = heads_tails[letter][1] - 1;
				}
			}
		}
	}

	heads_tails = resetIndices(heads_tails, heads_tails2);
/*
	cout << "after third pass: " << endl;
	for (size_t i = 0; i < text.size(); i++){
		cout << textSorted[i] << "|";
	}
	cout << endl;
*/

	return textSorted;
}

bool allDistinct(const vector<size_t>& vec){
	unordered_set<size_t> seen;

	for (size_t num : vec) {
		if (seen.find(num) != seen.end()) {
			return false;
		}
		seen.insert(num);
	}
	return true;
}
