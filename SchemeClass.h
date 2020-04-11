#pragma once
#include <string>
#include <iostream> 
#include <vector> 

using namespace std;

class SchemeClass {

public:
	SchemeClass(string name, vector<string> colNames);
	SchemeClass();
	void ToString();
	void AddRow(vector<string> row);
	string GetName();
	vector<string> GetColNames();
	void setColNames(vector<string> newNames);
	void AssessQuery(vector<string> query);
	vector<vector<string>> GetRelation();
	vector<string> GetRow(int numRow);
	int GetNumRows();
	int GetNumCols();
	SchemeClass Union(SchemeClass newRel, vector<vector<string>> rightSide, vector<string> leftSide, vector<string> colNames);
	//vector<string> Simplify(vector<vector<string>> currRightSide, vector<string> currLeftSide);
	void Select(vector<vector<string>> adding, vector<string> combinedColNames);
	void reorder();
	void reorder1(vector<string> order, vector<string> rightSide);
private:
	vector<vector<string>> relation;
	vector<string> columnNames;
	string RelationName;
	int numCols;
	int numRows;
	bool chill;
};