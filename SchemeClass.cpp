#include "SchemeClass.h"


SchemeClass::SchemeClass(string name, vector<string> colNames)
{
	RelationName = name;
	columnNames = colNames;
	numCols = colNames.size();
	numRows = 0;
	chill = false;

	return;
}
SchemeClass::SchemeClass()
{
	RelationName = "none";
	vector<string> placeHolder;
	placeHolder.push_back("none");
	columnNames = placeHolder;
	numCols = 0;
	numRows = 0;
	chill = false;

	return;
}

void SchemeClass::AddRow(vector<string> row)
{
	vector<string> toPush;
	int size = row.size();
	if (size <= numCols || chill == true)
	{
		chill = false;
		for (int i = 0; i < size; i++)
		{
			toPush.push_back(row[i]);
		}
	}
	else
	{
		for (int i = 1; i < size; i++)
		{
			toPush.push_back(row[i]);
		}
	}
	relation.push_back(toPush);
	numRows++;
	return;
}

int SchemeClass::GetNumRows()
{
	return relation.size();
}
int SchemeClass::GetNumCols()
{
	return numCols;
}

string SchemeClass::GetName()
{
	return RelationName;
}
vector<vector<string>> SchemeClass::GetRelation()
{
	return relation;
}
vector<string> SchemeClass::GetColNames()
{
	return columnNames;
}
vector<string> SchemeClass::GetRow(int numRow)
{
	vector<string> toReturn = relation[numRow];
	return toReturn;
}
void SchemeClass::AssessQuery(vector<string> query)
{
	vector<vector<string>> validRows;

	int length = query.size();
	bool validRow;
	vector<string> ColVars;
	vector<int> varPos;

	// First print Query
	cout << query[0] << "(";
	for (int i = 1; i < length; i++)
	{
		if (i != length - 1)
		{
			cout << query[i] << ",";
		}
		else
		{
			cout << query[i] << ")? ";
		}
	}

	// Checking for the valid rows

	for (int i = 0; i < numRows; i++)
	{
		validRow = true;
		int varNum = 0;
		for (int j = 1; j < length; j++)
		{
			string h = query[j];
			if (query[j].front() == '\'' && query[j] != relation[i][j - 1])
			{
				validRow = false;
			}
			else if (query[j].front() != '\'')
			{
				if (varNum != 0)
				{
					bool exists = false;
					for (int k = 0; k < varNum; k++)
					{
						if (query[j] == ColVars[k])
						{
							exists = true;
							if (relation[i][j - 1] != relation[i][varPos[k]])
							{
								validRow = false;
							}
						}
					}
					if (exists == false)
					{
						ColVars.push_back(query[j]);
						varPos.push_back(j - 1);
						varNum++;
					}

				}
				else
				{
					ColVars.push_back(query[j]);
					varPos.push_back(j - 1);
					varNum++;
				}
			}
		}

		if (validRow == true)
		{
			validRows.push_back(relation[i]);
		}
	}

	// Alphabetize validRows for some reason?
	int numValid = validRows.size();

	vector<vector<string>> tempVectVect;
	int smallestPos;
	for (int i = 0; i < numValid; i++)
	{
		int numLeft = validRows.size();
		for (int j = 0; j < numLeft; j++)
		{


			if (j == 0)
			{
				smallestPos = j;
			}
			else
			{
				if (validRows[j] < validRows[smallestPos])
				{
					smallestPos = j;
				}
			}
		}
		tempVectVect.push_back(validRows[smallestPos]);
		validRows.erase(validRows.begin() + smallestPos);

	}
	validRows = tempVectVect;


	numValid = validRows.size();

	if (numValid != 0)
	{
		cout << "Yes(" << numValid << ")" << endl;
		for (int i = 0; i < numValid; i++)
		{

			int s = varPos.size() / numRows;
			for (int j = 0; j < s; j++)
			{
				if (j == 0)
				{
					cout << "  ";
				}
				cout << ColVars[j] << "=" << validRows[i][varPos[j]];

				if (j != s - 1)
				{
					cout << ", ";
				}
			}
			if (s != 0)
			{
				cout << endl;
			}
		}
	}
	else
	{
		cout << "No" << endl;
	}

	return;
}


void SchemeClass::ToString()
{
	cout << RelationName << ":" << endl;
	for (int i = 0; i < numCols; i++)
	{
		cout << columnNames[i] << "\t";
	}

	cout << endl;

	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numCols; j++)
		{
			cout << relation[i][j] << "\t";
		}
		cout << endl;
	}

	return;
}

vector<vector<int>> identifyMatches(vector<string> oneCols, vector<string> twoCols)
{
	vector<vector<int>> toReturn;
	int oneSize = oneCols.size();
	for (int i = 0; i < oneSize; i++)
	{
		int twoSize = twoCols.size();
		for (int j = 0; j < twoSize; j++)
		{
			if (oneCols[i] == twoCols[j])
			{
				vector<int> toAdd;
				toAdd.push_back(i);
				toAdd.push_back(j);
				toReturn.push_back(toAdd);
			}
		}
	}
	return toReturn;
}

bool isMatch(vector<vector<int>> matches, int onePos, int twoPos)
{
	int numMatches = matches.size();
	for (int i = 0; i < numMatches; i++)
	{
		if (matches[i][0] == onePos && matches[i][1] == twoPos)
		{
			return true;
		}
	}
	return false;
}


SchemeClass SchemeClass::Union(SchemeClass newRel, vector<vector<string>> rightSide, vector<string> leftSide, vector<string> colNames)
{
	vector<vector<int>> matches = identifyMatches(rightSide[0], rightSide[1]);

	SchemeClass product("Result", colNames);

	for (int i = 0; i < numRows; i++)
	{
		int newNumRows = newRel.GetNumRows();
		for (int j = 0; j < newNumRows; j++)
		{
			bool valid = true;

			int s1 = relation[i].size();
			for (int k = 0; k < s1; k++)
			{
				int s2 = newRel.GetRow(j).size();
				for (int l = 0; l < s2; l++)
				{
					bool currValid = isMatch(matches, k, l);
					if (currValid == true && relation[i][k] != newRel.GetRow(j)[l])
					{
						valid = false;
					}

				}

			}

			if (valid == true)
			{
				bool stillValid = true;
				vector<string> newOne;
				vector<string> needed = leftSide;
				vector<string> found;
				int leftSize = needed.size();
				for (int x = 1; x < leftSize; x++)
				{
					
					int s1 = relation[i].size();
					for (int k = 0; k < s1; k++)
					{
						if (rightSide[0][k] == needed[x])
						{
							if (needed[x].at(0) == '\'' && relation[i][k] != needed[x])
							{
								stillValid = false;
								break;
							}

							newOne.push_back(relation[i][k]);
							found.push_back(needed[x]);
							needed[x] = "";

						}
					}
					if (stillValid == false)
					{
						break;
					}
					else {
						int s2 = newRel.GetRow(j).size();
						for (int k = 0; k < s2; k++)
						{
							if (rightSide[1][k] == needed[x])
							{
								if (needed[x].at(0) == '\'' && relation[i][k] != needed[x])
								{
									stillValid = false;
									break;
								}
								found.push_back(needed[x]);
								newOne.push_back(newRel.GetRow(j)[k]);
								needed[x] = "";
							}

						}
					}
					if (stillValid == false)
					{
						break;
					}
				}
				if (stillValid == true)
				{
					product.AddRow(newOne);
				}
				int i = found.size();
				if (i != 0)
				{
					product.setColNames(found);
				}
			}
		}
	}
	

	return product;
}

void SchemeClass::setColNames(vector<string> newNames) {
	columnNames = newNames;
	return;
}

/*

vector<string> SchemeClass::Simplify(vector<vector<string>> currRightSide, vector<string> currLeftSide)
{
	// Makes rightCols vector
	
	vector<string> rightCols;
	int rsSize = currRightSide.size();
	for (int i = 0; i < rsSize; i++)
	{
		int cSideSize = currRightSide[i].size();
		for (int j = 1; j < cSideSize; j++)
		{
			rightCols.push_back(currRightSide[i][j]);
		}
	}
	int leftSize = currLeftSide.size();
	rsSize = rightCols.size();
	for (int i = rsSize - 1; i >= 0; i--)
	{
		bool valid = false;
		for (int j = 0; j < leftSize; j++)
		{
			if (rightCols[i] == currLeftSide[j])
			{
				valid = true;
			}
		}
		if (valid == false)
		{
4n-			columnNames.erase(columnNames.begin() + i);
			rightCols.erase(rightCols.begin() + i);
			rsSize--;
			int numRels = relation.size();
			
			for (int k = 0; k < numRels; k++)
			{
				relation[k].erase(relation[k].begin() + i);
			}
			numCols--;
		}

	}
	
	

	// Now delete superfluous columns
	for (int i = 0; i < numCols - 1; i++)
	{
		for (int j = i + 1; j < numCols; j++)
		{
			if (rightCols[i] == rightCols[j])
			{
				columnNames.erase(columnNames.begin() + j);
				rightCols.erase(rightCols.begin() + j);
				int numRels = relation.size();
				for (int k = 0; k < numRels; k++)
				{
					relation[k].erase(relation[k].begin() + j);
				}
				j--;
				numCols--;
			}
		}
		
	}
	return rightCols;
}
*/

void SchemeClass::Select(vector<vector<string>> adding, vector<string> combinedColNames)
{
	for (int i = 0; i < numCols; i++)
	{
		int numComCols = combinedColNames.size();
		for (int j = 0; j < numComCols; j++)
		{
			if (columnNames[i] == combinedColNames[j])
			{
				int numCombRows = adding.size();
				for (int k = 0; k < numCombRows; k++)
				{
					if (i == 0)
					{
						vector<string> curr;
						curr.push_back(adding[k][j]);
						relation.push_back(curr);
					}
					else
					{
						relation[k].push_back(adding[k][j]);
					}
				}
			}
		}	
	}


	return;
}

void Alphabetize(vector<vector<string>> &rel)
{
	int rows = rel.size();
	for (int i = 0; i < rows - 1; i++)
	{
		vector<string> minIndex = rel[i];
		int minDexNum = i;

		for (int j = i + 1; j < rows; j++)
		{
			if (rel[j][0] < minIndex[0])
			{
				minIndex = rel[j];
				minDexNum = j;
			}
			else if (rel[j][0] == minIndex[0])
			{
				int numCols = rel[0].size();
				int numSame = 1;
				for (int k = 1; k < numCols; k++)
				{
					if (rel[j][k] > minIndex[k])
					{
						break;
					}
					else if (rel[j][k] < minIndex[k])
					{
						minIndex = rel[j];
						minDexNum = j;
						break;
					}
					else {
						numSame++;
					}
				}
				int jSize = rel[j].size();
				if (numSame == jSize)
				{
					rel.erase(rel.begin() + j);
					rows = rows - 1;
					j--;
				}
			}
		}
		vector<string> temp = rel[i];
		rel[i] = minIndex;
		rel[minDexNum] = temp;
	}

	return;

}

void SchemeClass::reorder()
{


	Alphabetize(relation);
	
	return;
}
void SchemeClass::reorder1(vector<string> order, vector<string> rightSide)
{
	vector<vector<string>> tempRelation;
	int numRows = relation.size();

	int o = order.size();
	vector<string> right;
	int c = rightSide.size();
	for (int j = 1; j < c; j++)
	{
		right.push_back(rightSide[j]);
	}
	for (int k = 0; k < numRows; k++)
	{
		bool valid = true;
		vector<string> newTuple;
		for (int i = 0; i < o; i++)
		{
			int cols = right.size();
			for (int j = 0; j < cols; j++)
			{
				if (columnNames[j] == order[i])
				{


					for (int x = 0; x < cols; x++)
					{
						if (right[x].at(0) == '\'' && right[x] != relation[k][x])
						{
							valid = false;
						}
					}
					if (valid == true)
					{
						newTuple.push_back(relation[k][j]);
					}

				}
			}
		}
		int mm = newTuple.size();
		if (mm != 0)
		{
			tempRelation.push_back(newTuple);
		}
	}
	Alphabetize(tempRelation);
	relation = tempRelation;
	//columnNames = order;
	numCols = o;
	return;
}
