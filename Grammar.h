#ifndef GRAMMAR_H
#define GRAMMAR_H
#include <iostream> 
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector> 
#include <algorithm>
#include "SchemeClass.h"

using namespace std;

string ErrorGenerator(bool &isError, string currTok, string id, int lineNum, string errorMessage)
{
	if (isError == false)
	{
		isError = true;
		ostringstream str1;
		str1 << "(" << currTok << ",\"" << id << "\"," << lineNum << ")";
		string final = str1.str();
		return final;
	}
	else
	{
		return errorMessage;
	}

}

// Begin SCHEME stuff#####################################################################################################################################
void IDList(ifstream &in, int &schemeNum, string &id, string &currTok, vector<string> &toPush, bool &isError, string &errorMessage, int &lineNum)
{
	currTok = PrintFinal(in, id, lineNum);
	if (currTok == "COMMA")
	{
		currTok = PrintFinal(in, id, lineNum);
		if (currTok == "ID")
		{
			toPush.push_back(id);
			IDList(in, schemeNum, id, currTok, toPush, isError, errorMessage, lineNum);
		}
		else
		{
			errorMessage = ErrorGenerator(isError, currTok, id, lineNum, errorMessage);

			return;
		}
	}
	else
	{
		return;
	}
}

void Scheme(ifstream &in, int &schemeNum, string &id, string &currTok, vector <SchemeClass> &schemes, bool &isError, string &errorMessage, int &lineNum)
{
	if (currTok != "ID")
	{
		currTok = PrintFinal(in, id, lineNum);
	}
	string relName = id;
	vector<string> toPush;
	if (currTok == "ID" && PrintFinal(in, id, lineNum) == "LEFT_PAREN")
	{
		currTok = PrintFinal(in, id, lineNum);
		if (currTok == "ID")
		{

			toPush.push_back(id);
			IDList(in, schemeNum, id, currTok, toPush, isError, errorMessage, lineNum);
			if (currTok == "RIGHT_PAREN")
			{
				SchemeClass newRel(relName, toPush);
				schemes.push_back(newRel);
				return;
			}
			else
			{
				errorMessage = ErrorGenerator(isError, currTok, id, lineNum, errorMessage);
				return;
			}
		}
		else
		{
			errorMessage = ErrorGenerator(isError, currTok, id, lineNum, errorMessage);
			return;
		}
	}
	else
	{
		errorMessage = ErrorGenerator(isError, currTok, id, lineNum, errorMessage);
		return;
	}

}

void SchemeList(ifstream &in, int &schemeNum, string &id, string &currTok, vector <SchemeClass> &schemes, int nextStack, bool &isError, string &errorMessage, int &lineNum)
{
	string schemeCount = "Schemes(";

	currTok = PrintFinal(in, id, lineNum);
	if (currTok == "FACTS" || currTok == "QUERIES" || currTok == "RULES")
	{
		if (currTok == "FACTS")
		{

		}
		else
		{
			errorMessage = ErrorGenerator(isError, currTok, id, lineNum, errorMessage);
		}

		return;
	}
	else {
		Scheme(in, schemeNum, id, currTok, schemes, isError, errorMessage, lineNum);
		SchemeList(in, schemeNum, id, currTok, schemes, nextStack, isError, errorMessage, lineNum);
		return;
	}
}
// End SCHEME stuff #######################################################################################################################################

// Begin FACT stuff #######################################################################################################################################
void StringList(ifstream &in, int &factNum, string &id, string &currTok, vector<string> &toPush, vector<string> &elements, bool &isError, string &errorMessage, int &lineNum)
{
	currTok = PrintFinal(in, id, lineNum);
	if (currTok == "COMMA")
	{
		currTok = PrintFinal(in, id, lineNum);
		if (currTok == "STRING")
		{
			//For Domain
			bool alreadyExists = false;
			int numElements = elements.size();
			for (int i = 0; i < numElements; i++)
			{
				if (elements[i] == id)
				{
					alreadyExists = true;
				}
			}
			if (alreadyExists == false)
			{
				elements.push_back(id);
			}

			toPush.push_back(id);
			StringList(in, factNum, id, currTok, toPush, elements, isError, errorMessage, lineNum);
		}
		else
		{
			errorMessage = ErrorGenerator(isError, currTok, id, lineNum, errorMessage);
			return;
		}
	}
	else
	{
		return;
	}
}

void Fact(ifstream &in, int &factNum, string &id, string &currTok, vector <vector<string>> &facts, vector<string> &elements, bool &isError, string &errorMessage, int &lineNum)
{
	if (currTok != "ID")
	{
		currTok = PrintFinal(in, id, lineNum);
	}
	vector<string> toPush;
	toPush.push_back(id);
	if (currTok == "ID" && PrintFinal(in, id, lineNum) == "LEFT_PAREN" && PrintFinal(in, id, lineNum) == "STRING")
	{
		//For Domain
		bool alreadyExists = false;
		int numElements = elements.size();
		for (int i = 0; i < numElements; i++)
		{
			if (elements[i] == id)
			{
				alreadyExists = true;
			}
		}
		if (alreadyExists == false)
		{
			elements.push_back(id);
		}

		toPush.push_back(id);
		StringList(in, factNum, id, currTok, toPush, elements, isError, errorMessage, lineNum);
		if (currTok == "RIGHT_PAREN" && PrintFinal(in, id, lineNum) == "PERIOD")
		{
			facts.push_back(toPush);
			factNum++;
			return;
		}
		else
		{
			errorMessage = ErrorGenerator(isError, currTok, id, lineNum, errorMessage);
			return;
		}
	}
	else
	{
		errorMessage = ErrorGenerator(isError, currTok, id, lineNum, errorMessage);
		return;
	}
}
void FactList(ifstream &in, int &factNum, string &id, string &currTok, vector <vector<string>> &facts, int nextStack, vector<string> &elements, bool &isError, string &errorMessage, int &lineNum)
{
	string factCount = "Facts(";
	currTok = PrintFinal(in, id, lineNum);
	if (currTok == "RULES" || currTok == "QUERIES")
	{
		if (currTok == "RULES")
		{
			return;
		}
		else
		{
			errorMessage = ErrorGenerator(isError, currTok, id, lineNum, errorMessage);
			return;
		}
	}
	else
	{
		Fact(in, factNum, id, currTok, facts, elements, isError, errorMessage, lineNum);
		FactList(in, factNum, id, currTok, facts, nextStack, elements, isError, errorMessage, lineNum);
		return;
	}
}

// End FACT stuff ##########################################################################################################################################

// Begin RULE stuff ########################################################################################################################################

void RealOperator(ifstream &in, int &ruleNum, string &id, string &currTok, vector<string> &toPush, bool &isError, string &errorMessage, int &lineNum)
{
	currTok = PrintFinal(in, id, lineNum);
	if (currTok == "ADD")
	{
		toPush.push_back(id);
		return;
	}
	else if (currTok == "MULTIPLY")
	{
		toPush.push_back(id);
		return;
	}
	else
	{
		errorMessage = ErrorGenerator(isError, currTok, id, lineNum, errorMessage);
		return;
	}
}

void RealParameter(ifstream &in, int &ruleNum, string &id, string &currTok, vector<string> &toPush, bool &isError, string &errorMessage, int &lineNum)
{
	currTok = PrintFinal(in, id, lineNum);
	if (currTok == "STRING")
	{
		toPush.push_back(id);
		return;
	}
	else if (currTok == "ID")
	{
		toPush.push_back(id);
		return;
	}
	else if (currTok == "LEFT_PAREN")
	{
		toPush.push_back(id);
		RealParameter(in, ruleNum, id, currTok, toPush, isError, errorMessage, lineNum);
		RealOperator(in, ruleNum, id, currTok, toPush, isError, errorMessage, lineNum);
		RealParameter(in, ruleNum, id, currTok, toPush, isError, errorMessage, lineNum);
		if (PrintFinal(in, id, lineNum) == "RIGHT_PAREN")
		{
			toPush.push_back(id);
			return;
		}
		else
		{
			errorMessage = ErrorGenerator(isError, currTok, id, lineNum, errorMessage);
			return;
		}

	}
}

void RealParameterList(ifstream &in, int &ruleNum, string &id, string &currTok, vector<string> &toPush, bool &isError, string &errorMessage, int &lineNum)
{
	currTok = PrintFinal(in, id, lineNum);
	if (currTok == "COMMA")
	{
		RealParameter(in, ruleNum, id, currTok, toPush, isError, errorMessage, lineNum);
		RealParameterList(in, ruleNum, id, currTok, toPush, isError, errorMessage, lineNum);
	}
	else
	{
		return;
	}
}

void RealPredicate(ifstream &in, int &ruleNum, string &id, string &currTok, vector<string> &toPush, bool &isError, string &errorMessage, int &lineNum)
{
	if (currTok != "ID")
	{
		currTok = PrintFinal(in, id, lineNum);
	}
	string currID = id;
	toPush.push_back(currID);
	if (currTok == "ID" && PrintFinal(in, id, lineNum) == "LEFT_PAREN")
	{
		RealParameter(in, ruleNum, id, currTok, toPush, isError, errorMessage, lineNum);
		RealParameterList(in, ruleNum, id, currTok, toPush, isError, errorMessage, lineNum);
	}
	if (currTok == "RIGHT_PAREN")
	{
		return;
	}
	else
	{
		if (isError == false)
		{
			errorMessage = ErrorGenerator(isError, currTok, id, lineNum, errorMessage);
		}

		return;
	}
}
void PredicateList(ifstream &in, int &ruleNum, string &id, string &currTok, vector<string> &toPush, bool &isError, string &errorMessage, int &lineNum)
{
	currTok = PrintFinal(in, id, lineNum);
	if (currTok == "COMMA")
	{
		toPush.push_back("|");
		RealPredicate(in, ruleNum, id, currTok, toPush, isError, errorMessage, lineNum);
		PredicateList(in, ruleNum, id, currTok, toPush, isError, errorMessage, lineNum);
	}
	else
	{
		return;
	}
}
void HeadPredicate(ifstream &in, int &ruleNum, string &id, string &currTok, vector<string> &toPush, bool &isError, string &errorMessage, int &lineNum)
{
	toPush.push_back(id);
	vector<string> PlaceHolder;
	if (currTok == "ID" && PrintFinal(in, id, lineNum) == "LEFT_PAREN" && PrintFinal(in, id, lineNum) == "ID")
	{
		toPush.push_back(id);
		IDList(in, ruleNum, id, currTok, toPush, isError, errorMessage, lineNum);
		if (currTok == "RIGHT_PAREN")
		{
			return;
		}
		else
		{
			errorMessage = ErrorGenerator(isError, currTok, id, lineNum, errorMessage);
			return;
		}
	}
	else
	{
		errorMessage = ErrorGenerator(isError, currTok, id, lineNum, errorMessage);
		return;
	}
}
void Rule(ifstream &in, int &ruleNum, string &id, string &currTok, vector <vector<string>> &rules, bool &isError, string &errorMessage, int &lineNum)
{
	vector<string> toPush;

	HeadPredicate(in, ruleNum, id, currTok, toPush, isError, errorMessage, lineNum);
	currTok = PrintFinal(in, id, lineNum);
	if (currTok == "COLON_DASH")
	{
		toPush.push_back(":-");
		RealPredicate(in, ruleNum, id, currTok, toPush, isError, errorMessage, lineNum);
		PredicateList(in, ruleNum, id, currTok, toPush, isError, errorMessage, lineNum);

	}
	if (currTok == "PERIOD")
	{
		ruleNum++;
		toPush.push_back("|");
		rules.push_back(toPush);
		return;
	}
	else
	{
		errorMessage = ErrorGenerator(isError, currTok, id, lineNum, errorMessage);
		return;
	}

}
void RuleList(ifstream &in, int &ruleNum, string &id, string &currTok, vector <vector<string>> &rules, int nextStack, bool &isError, string &errorMessage, int &lineNum)
{
	string ruleCount = "Rules(";

	currTok = PrintFinal(in, id, lineNum);
	if (currTok == "QUERIES")
	{
		return;
	}
	else
	{
		Rule(in, ruleNum, id, currTok, rules, isError, errorMessage, lineNum);
		if (isError == false)
		{
			RuleList(in, ruleNum, id, currTok, rules, nextStack, isError, errorMessage, lineNum);
		}

	}
}
// End RULE stuff ##########################################################################################################################################

// Begin QUERIES stuff #####################################################################################################################################
void Query(ifstream &in, int &queryNum, string &id, string &currTok, vector <vector<string>> &queries, bool &isError, string &errorMessage, int &lineNum)
{
	vector<string> toPush;
	if (currTok == "ID")
	{
		RealPredicate(in, queryNum, id, currTok, toPush, isError, errorMessage, lineNum);
		currTok = PrintFinal(in, id, lineNum);
		if (currTok == "Q_MARK")
		{
			queries.push_back(toPush);
			queryNum++;
			return;
		}
		else
		{
			errorMessage = ErrorGenerator(isError, currTok, id, lineNum, errorMessage);
			return;
		}
	}
	else
	{
		errorMessage = ErrorGenerator(isError, currTok, id, lineNum, errorMessage);
		return;
	}
}
void QueryList(ifstream &in, int &queryNum, string &id, string &currTok, vector <vector<string>> &queries, int nextStack, bool &isError, string &errorMessage, int &lineNum)
{
	currTok = PrintFinal(in, id, lineNum);
	if (currTok == "EOF")
	{
		return;
	}
	else
	{
		Query(in, queryNum, id, currTok, queries, isError, errorMessage, lineNum);
		QueryList(in, queryNum, id, currTok, queries, nextStack, isError, errorMessage, lineNum);
	}
}
// End QUERIES stuff #######################################################################################################################################

// Begin Domain stuff ######################################################################################################################################
void VectorSort(vector<string> &elements)
{
	int size = elements.size();
	string placeholder;
	for (int i = 0; i < size; i++)
	{
		for (int j = i; j < size; j++)
		{
			if (elements[i] > elements[j])
			{
				placeholder = elements[i];
				elements[i] = elements[j];
				elements[j] = placeholder;
			}
		}
	}
	return;
}

void Domain(vector<string> &elements, vector <string> &domain)
{
	VectorSort(elements);

	int size = elements.size();

	ostringstream str1;
	str1 << "Domain(" << size << "):";
	string toPush = str1.str();

	domain.push_back(toPush);

	for (int i = 0; i < size; i++)
	{
		string pushed = "  " + elements[i];
		domain.push_back(pushed);
	}

	return;
}
// end Domain stuff ########################################################################################################################################

// Funtion for adding getting from facts to adding rows

void addRows(vector<SchemeClass> &relations, vector<vector<string>> toAdd)
{
	int numRels = relations.size();
	int numFacts = toAdd.size();

	for (int i = 0; i < numRels; i++)
	{
		for (int j = 0; j < numFacts; j++)
		{
			if (relations[i].GetName() == toAdd[j][0])
			{
				vector<vector<string>> relation = relations[i].GetRelation();

				// same as toAdd but without names at the front

				vector<vector<string>> modToAdd;
				for (int k = 0; k < numFacts; k++)
				{
					vector<string> element;
					int currSize = toAdd[k].size();
					for (int l = 1; l < currSize; l++)
					{
						element.push_back(toAdd[k][l]);
					}
					modToAdd.push_back(element);
				}


				bool exists = false;
				int relRows = relation.size();
				for (int k = 0; k < relRows; k++)
				{
					if (relation[k] == modToAdd[j])
					{
						exists = true;
					}
				}
				if (exists == false)
				{
					relations[i].AddRow(toAdd[j]);
				}
			}
		}
	}
	return;

}

// Function for assessing queries
void AssessQueries(vector<SchemeClass> &relations, vector<vector<string>> &queries)
{
	int numRels = relations.size();
	int numQuers = queries.size();

	for (int j = 0; j < numQuers; j++)
	{
		for (int i = 0; i < numRels; i++)
		{
			if (relations[i].GetName() == queries[j][0])
			{
				relations[i].AssessQuery(queries[j]);
			}
		}
	}
	return;
}
// Function for assessing rules
SchemeClass MakeValidCopy(SchemeClass original, vector<string> rs)
{
	bool hasQuote = false;
	int rsSize = rs.size();
	for (int i = 0; i < rsSize; i++)
	{
		if (rs[i].at(0) == '\'')
		{
			hasQuote = true;
		}
	}
	if (hasQuote == true)
	{
		SchemeClass copy("copy", original.GetColNames());
		int ogSize = original.GetRelation().size();
		for (int i = 0; i < ogSize; i++)
		{
			bool valid = true;
			int numCols = original.GetNumCols();
			for (int j = 0; j < numCols; j++)
			{
				if (rs[j + 1].at(0) == '\'' && original.GetRelation()[i][j] != rs[j + 1])
				{
					valid = false;
				}
			}
			if (valid == true)
			{
				copy.AddRow(original.GetRelation()[i]);
			}
		}
		return copy;
	}
	else {
		return original;
	}

}
void Assignment(vector<SchemeClass> &currRels, vector<vector<string>> &adding,  SchemeClass &ruleRel, vector<string> order, vector<vector<string>> &currRightSide, vector<string> currLeftSide)
{
	if (currRels.size() > 1)
	{
		vector<vector<string>> unionColNames;

		int currsize = currRightSide[0].size();
		vector<string> one;
		for (int i = 1; i < currsize; i++)
		{
			one.push_back(currRightSide[0][i]);
		}
		unionColNames.push_back(one);

		currsize = currRightSide[1].size();
		vector<string> two;
		for (int i = 1; i < currsize; i++)
		{
			two.push_back(currRightSide[1][i]);
		}
		unionColNames.push_back(two);

		SchemeClass rels0copy = MakeValidCopy(currRels[0], currRightSide[0]);
		SchemeClass rels1copy = MakeValidCopy(currRels[1], currRightSide[1]);

		SchemeClass combined1 = rels0copy.Union(rels1copy, unionColNames, currLeftSide, ruleRel.GetColNames());
		int numRels = currRels.size();
		if (numRels > 2)
		{
			int numTogether = 2;
			for (int i = 2; i < numRels; i++)
			{
				vector<vector<string>> unionColNamesL;
				int currsize;
				vector<string> one;

				for (int j = 0; j < numTogether; j++)
				{
					currsize = currRightSide[j].size();
					for (int i = 1; i < currsize; i++)
					{
						one.push_back(currRightSide[j][i]);
					}
					
				}
				vector<string> abc = combined1.GetColNames();
				unionColNamesL.push_back(abc);

				currsize = currRightSide[numTogether].size();
				vector<string> two;
				for (int i = 1; i < currsize; i++)
				{
					two.push_back(currRightSide[numTogether][i]);
				}
				
				unionColNamesL.push_back(two);
				SchemeClass currRelICopy = MakeValidCopy(currRels[i], currRightSide[i]);
				combined1 = combined1.Union(currRelICopy,unionColNamesL, currLeftSide, ruleRel.GetColNames());
				numTogether++;
			}
		}
		//order = combined1.Simplify(currRightSide, currLeftSide);
		if (combined1.GetRelation().size() == 0)
		{
			return;
		}

		//combined1.reorder(order, currLeftSide);
		combined1.reorder();
		int resRows = combined1.GetNumRows();

		for (int i = 0; i < resRows; i++)
		{

			bool already = false;
			int adSize = adding.size();
			if (i != 0)
			{
				if (combined1.GetRelation()[i] == combined1.GetRelation()[i - 1])
				{
					already = true;
				}
				else {
					for (int j = 0; j < adSize; j++)
					{
						if (combined1.GetRelation()[i] == adding[j])
						{
							already = true;
						}
					}
				}
			}
			if (already != true)
			{
				adding.push_back(combined1.GetRelation()[i]);
			}
		}
	}
	else
	{
		SchemeClass combined1 = currRels[0];

		int resRows = combined1.GetNumRows();
		
		

		if (combined1.GetRelation().size() == 0)
		{
			return;
		}

		combined1.reorder1(order, currRightSide[0]);
		///////////////////////////////////
		resRows = combined1.GetNumRows();
		for (int i = 0; i < resRows; i++)
		{
			vector<string> currRow = combined1.GetRelation()[i];
			
			bool already = false;
			int adSize = adding.size();
			for (int j = 0; j < adSize; j++)
			{
				if (currRow == adding[j])
				{
					already = true;
				}
			}
			if (already != true)
			{
				adding.push_back(currRow);
			}
		}
		
	}
	return;
}

bool isNew(vector<vector<string>> &original, vector<vector<string>> &newOne)
{
	bool added = false;
	int newSize = newOne.size();
	int oldSize = original.size();
	for (int i = 0; i < newSize; i++)
	{
		bool found = false;
		for (int j = 0; j < oldSize; j++)
		{
			if (original[j] == newOne[i])
			{
				found = true;
			}
		}
		if (found == false)
		{
			added = true;
		}
	}
	return added;
}
void RemoveDups(vector<vector<string>> &original, vector<vector<string>> &newOne)
{
	int newSize = newOne.size();
	int oldSize = original.size();
	for (int i = newSize - 1; i >= 0; i--)
	{
		for (int j = 0; j < oldSize; j++)
		{
			if (original[j] == newOne[i])
			{
				newOne.erase(newOne.begin() + i);
				break;
			}
		}
		
	}

	return;
}
void AssessCurrRule(vector<string> &currLeftSide, vector<vector<string>> &currRightSide, SchemeClass &ruleRel, vector<SchemeClass> &currRightRels)
{
	//int pSize = currRightSide.size();
	//vector<SchemeClass> currRels;
	//vector<vector<bool>> isValid;
	//vector<string> invalidEls;

	/*
	if (currRightRels.size() == 1)
	{
		int nummElls = currRightSide[0].size();
		bool complex = false;
		for (int i = 1; i < nummElls; i++)
		{
			if (currRightSide[0][i].at(0) == '\'')
			{
				complex = true;
				SchemeClass newRel(currRightRels[0].GetName(), currRightRels[0].GetColNames());
				int relRows = currRightRels[0].GetNumRows();
				for (int l = 0; l < relRows; l++)
				{
					if (currRightRels[0].GetRow(l)[i - 1] == currRightSide[0][i])
					{
						newRel.AddRow(currRightRels[0].GetRow(l));
					}
				}
				currRels.push_back(newRel);
			}
		}
		if (complex == false)
		{
			currRels.push_back(currRightRels[0]);
		}
	}
	
	else
	{
		
		// OK
		for (int i = 0; i < pSize; i++)
		{
			vector<bool> rowValidity;
			int rows = currRightRels[i].GetNumRows();
			for (int j = 0; j < rows; j++)
			{
				rowValidity.push_back(true);
			}
			isValid.push_back(rowValidity);
		}
		// OK
		//Check through right sides      14,811
		
		for (int i = 0; i < pSize - 1; i++)
		{
			int rows1 = currRightRels[i].GetNumRows();
			// Check through all right sides after the ith right side
			for (int j = i + 1; j < pSize; j++)
			{
				int rows2 = currRightRels[j].GetNumRows();
				int numEls = currRightRels[i].GetNumCols();
				// Check through elements of the ith right side
				for (int k = 0; k < numEls; k++)
				{
					int numRowsMatched = 0;
					if (currRightSide[i][k+1].at(0) == '\'')
					{
						for (int x = 0; x < rows1; x++)
						{
							if (currRightRels[i].GetRow(x)[k] != currRightSide[i][k + 1])
							{
								isValid[i][x] = false;
							}
							
						}
					}
					// if the element has already had no matches in the past, make the row false
					int numElsNext = currRightRels[j].GetNumCols();
					// Check through elements of the jth right side
					for (int l = 0; l < numElsNext; l++)
					{
						if (currRightSide[j][l+1].at(0) == '\'')
						{
							for (int y = 0; y < rows2; y++)
							{

								if (currRightRels[j].GetRow(y)[l] != currRightSide[j][l + 1])
								{
									isValid[j][y] = false;
								}


							}
						}
						// if 2 elements match, check if the rows match
						if (currRightSide[i][k+1] == currRightSide[j][l+1])
						{
							
							for (int x = 0; x < rows1; x++)
							{
								if (currRightSide[i][k].at(0) == '\'')
								{
									if (currRightRels[i].GetRow(x)[k] != currRightSide[i][k])
									{
										isValid[i][x] = false;
									}
								}
								
								int numMatches = 0;
								
								
								
								for (int y = 0; y < rows2; y++)
								{
									if (i == pSize - 2)
									{
										if (currRightSide[j][l].at(0) == '\'')
										{
											if (currRightRels[j].GetRow(y)[l] != currRightSide[j][l])
											{
												isValid[j][y] = false;
											}
										}
									}
									if (currRightRels[i].GetRow(x)[k] == currRightRels[j].GetRow(y)[l])
									{
										if (isValid[i][x] != false && isValid[j][y])
										{
											numMatches++;
										}
									}	

								}

								if (numMatches == 0)
								{
									isValid[i][x] = false;
									numRowsMatched++;
								
								}
							}
						}
					}
				}
			}
		}
		
		//OK  18,273
		for (int i = pSize - 1; i > 0; i--)
		{
			// Check through all right sides after the ith right side
			for (int j = i - 1; j >= 0; j--)
			{
				int numEls = currRightRels[i].GetNumCols();
				// Check through elements of the ith right side
				for (int k = 0; k < numEls; k++)
				{
					int numRowsMatched = 0;
					// if the element has already had no matches in the past, make the row false
					int numElsNext = currRightRels[j].GetNumCols();
					// Check through elements of the jth right side
					for (int l = 0; l < numElsNext; l++)
					{
						// if 2 elements match, check if the rows match
						if (currRightSide[i][k + 1] == currRightSide[j][l + 1])
						{
							int rows1 = currRightRels[i].GetNumRows();
							for (int x = 0; x < rows1; x++)
							{
								int numMatches = 0;
								int rows2 = currRightRels[j].GetNumRows();


								for (int y = 0; y < rows2; y++)
								{
									if (currRightRels[i].GetRow(x)[k] == currRightRels[j].GetRow(y)[l])
									{
										if (isValid[i][x] != false && isValid[j][y] != false)
										{
											numMatches++;
										}
									}

								}

								if (numMatches == 0)
								{
									isValid[i][x] = false;
									numRowsMatched++;

								}
							}
						}
					}
				}
			}
		}
		// OK
		for (int i = 0; i < pSize; i++)
		{
			int rows = isValid[i].size();
			SchemeClass neww(currRightRels[i].GetName(), currRightRels[i].GetColNames());
			for (int j = 0; j < rows; j++)
			{
				if (isValid[i][j] == true)
				{
					neww.AddRow(currRightRels[i].GetRow(j));
				}
			}
			currRels.push_back(neww);
		}
		
			
	}
	*/

	

	vector<vector<string>> adding;
	vector<string> currEl;
	vector<vector<string>> toSatisfy;
	vector<vector<string>> satCols;
	vector<string> order;

	int leftEls = currLeftSide.size();
	//OK
	for (int i = 1; i < leftEls; i++)
	{
		order.push_back("Placeholder");
	}
	int nummRights = currRightSide.size();
	//OK
	if (nummRights == 1)
	{
		int numCurrRights = currRightSide[0].size();
		for (int k = 1; k < numCurrRights; k++)
		{
			for (int i = 1; i < leftEls; i++)
			{
				if (currLeftSide[i] == currRightSide[0][k])
				{
					order[k - 1] = currRightRels[0].GetColNames()[k - 1];
				}
			}
		}

	}

	//OK
	for (int i = 1; i < leftEls; i++)
	{

		int numRights = currRightSide.size();
		for (int j = 0; j < numRights; j++)
		{
			int numCurrRights = currRightSide[j].size();
			for (int k = 1; k < numCurrRights; k++)
			{
				if (currLeftSide[i] == currRightSide[j][k])
				{
					order[i - 1] = currRightRels[j].GetColNames()[k - 1];
				}
			}
		}
	}

	
	int begSize = ruleRel.GetNumRows();
	// HERE
	Assignment(currRightRels, adding, ruleRel, order, currRightSide, currLeftSide);


	int endSize = ruleRel.GetNumRows();

	

	if (begSize != endSize)
	{
		sort(adding.begin(), adding.begin() + endSize - begSize);
	}
	
	// alphabetize adding
	
	if (isNew(ruleRel.GetRelation(),adding) == true)
	{
		RemoveDups(ruleRel.GetRelation(), adding);
		int rowsToAss = adding.size();
		for (int i = 0; i < rowsToAss; i++)
		{
			//bool printed = false;
			ruleRel.AddRow(adding[i]);
			cout << "  ";
			int cols = ruleRel.GetNumCols();

			for (int k = 0; k < cols; k++)
			{


				cout << ruleRel.GetColNames()[k] << "=" << adding[i][k];
				if (k == cols - 1)
				{
					cout << endl;
				}
				else
				{
					cout << ", ";
				}
				//printed = true;

			}
		}
	}

	

	
	return;
}
void AssessRules(vector<vector<string>> leftSide, vector<vector<vector<string>>> rightSide, vector<SchemeClass> &relations, int &passes, int prevChanges)
{
	int numRules = leftSide.size();
	int numRels = relations.size();
	int changes = 0;
	for (int i = 0; i < numRules; i++)
	{
		for (int j = 0; j < numRels; j++)
		{
			if (leftSide[i][0] == relations[j].GetName())
			{
				vector<string> currLeftSide = leftSide[i]; 
				vector<vector<string>> currRightSide = rightSide[i]; 
				SchemeClass ruleRel = relations[j];

				vector<SchemeClass> currRightRels;
				int mmm = rightSide[i].size();
				for (int k = 0; k < mmm; k++)
				{
					for (int l = 0; l < numRels; l++)
					{
						if (rightSide[i][k][0] == relations[l].GetName())
						{
							currRightRels.push_back(relations[l]);
						}
					}
				}
				int left = currLeftSide.size();
				for (int k = 0; k < left; k++)
				{
					cout << currLeftSide[k];
					if (k == 0)
					{
						cout << "(";
					}
					else if (k == left - 1)
					{
						cout << ") :- ";
					}
					else
					{
						cout << ",";
					}
				}

				int right = currRightSide.size();
				for (int k = 0; k < right; k++)
				{
					int cRight = currRightSide[k].size();
					for (int l = 0; l < cRight; l++)
					{
						cout << currRightSide[k][l];
						if (l == 0)
						{
							cout << "(";
						}
						else if (l == cRight - 1)
						{
							cout << ")";
							if (k == right - 1)
							{
								cout << "." << endl;
							}
							else
							{
								cout << ",";
							}
						}
						else
						{
							cout << ",";
						}
					}
				}

				int before = ruleRel.GetNumRows();
				AssessCurrRule(currLeftSide, currRightSide, ruleRel, currRightRels);
				relations[j] = ruleRel;
				int after = ruleRel.GetNumRows();
				if (before < after)
				{
					changes++;
				}
			}
			
		}
	}
	if (changes != 0)
	{
		passes++;
		AssessRules(leftSide, rightSide, relations, passes, changes);
	}
	return;
}

// DataLogProgram ##########################################################################################################################################
void DataLogProgram(ifstream &in)
{
	string id = "";
	int schemeNum = 0;
	string currTok;
	vector <string> output;
	int lineNum = 1;

	bool isError = false;
	string errorMessage = "none";

	// SCHEMES
	currTok = PrintFinal(in, id, lineNum);
	vector<SchemeClass> relations;
	if (currTok == "SCHEMES" && PrintFinal(in, id, lineNum) == "COLON" && isError == false)
	{

		output.push_back("schemes-n");
		int nextStack = output.size() - 1;

		Scheme(in, schemeNum, id, currTok, relations, isError, errorMessage, lineNum);
		if (isError == false)
		{
			SchemeList(in, schemeNum, id, currTok, relations, nextStack, isError, errorMessage, lineNum);
		}
	}
	else
	{
		errorMessage = ErrorGenerator(isError, currTok, id, lineNum, errorMessage);
	}

	// FACTS
	int factNum = 0;
	vector<string> elements;
	vector<vector<string>> facts;
	if (currTok == "FACTS" && PrintFinal(in, id, lineNum) == "COLON" && isError == false)
	{
		output.push_back("schemes-n");
		int nextStack = output.size() - 1;

		FactList(in, factNum, id, currTok, facts, nextStack, elements, isError, errorMessage, lineNum);
	}
	else
	{
		errorMessage = ErrorGenerator(isError, currTok, id, lineNum, errorMessage);
	}

	addRows(relations, facts);


	// RULES
	int ruleNum = 0;
	vector<vector<string>> rules;
	if (currTok == "RULES" && PrintFinal(in, id, lineNum) == "COLON" && isError == false)
	{
		output.push_back("rules-n");
		int nextStack = output.size() - 1;

		RuleList(in, ruleNum, id, currTok, rules, nextStack, isError, errorMessage, lineNum);
	}
	else
	{
		errorMessage = ErrorGenerator(isError, currTok, id, lineNum, errorMessage);
	}


	cout << "Rule Evaluation" << endl;

	vector<vector<string>> leftSide;
	vector<vector<vector<string>>> rightSide;


	int numRules = rules.size();
	for (int i = 0; i < numRules; i++)
	{
		vector<string> left;
		vector<vector<string>> right;

		int lastPos = 0;
		int ruleSize = rules[i].size();
		for (int j = 0; j < ruleSize; j++)
		{
			vector<string> r;

			if (rules[i][j] == ":-")
			{
				for (int k = lastPos; k < j; k++)
				{
					left.push_back(rules[i][k]);
				}
				lastPos = j;

				//break;
			}
			if (rules[i][j] == "|")
			{
				for (int k = lastPos + 1; k < j; k++)
				{
					r.push_back(rules[i][k]);
				}
				lastPos = j;
				right.push_back(r);
			}

		}

		leftSide.push_back(left);
		rightSide.push_back(right);
	}
	int passes = 1;
	AssessRules(leftSide, rightSide, relations, passes, 999999);
	cout << endl << "Schemes populated after " << passes << " passes through the Rules." << endl << endl;

	// QUERIES
	cout << "Query Evaluation" << endl;

	int queryNum = 0;
	vector<vector<string>> queries;

	if (currTok == "QUERIES" && PrintFinal(in, id, lineNum) == "COLON" && isError == false)
	{
		output.push_back("queries-n");
		int nextStack = output.size() - 1;

		QueryList(in, queryNum, id, currTok, queries, nextStack, isError, errorMessage, lineNum);
	}
	else
	{
		errorMessage = ErrorGenerator(isError, currTok, id, lineNum, errorMessage);
	}

	AssessQueries(relations, queries);



}
#endif
