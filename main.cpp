#include <iostream>
#include <algorithm>
#include <string>
using namespace std;

class UtilityList
{
public:
    string item;
    int size = 3;
    int index = 0;
    int **list;
    UtilityList(string it)
    {
        item = it;
        list = new int*[size];
        for (int i = 0; i < size; i++)
        {
            list[i] = new int[3]{0};
        }
    }
    void append(int tid, int iu, int ru)
    {
        if (index >= size)
        {
            int newSize = size*2;
            int **temp = new int*[newSize];
            for (int i = 0; i < index; i++){
                temp[i] = list[i];
            }
            for (int i = index; i < newSize; i++)
            {
                temp[i] = new int[3]{0};
            }
            delete [] list;
            list = temp;
            size = newSize;
        }
        list[index][0] = tid;
        list[index][1] = iu;
        list[index++][2] = ru;
    }
    void showList()
    {
        cout << "=====" << item << "=====" << endl;
        cout << index << endl;
        for (int i = 0; i < index; i++)
        {
            cout << list[i][0] << ", " << list[i][1] << ", " << list[i][2] << endl;
        }
        cout << "=====" << item << "=====" << endl;
    }
};

UtilityList Construct(int **PUL, int **PxUL, int **PyUL, int indexOfPUL, int indexOfPxUL, int indexOfPyUL, string itemX, string itemY)
{

    string itemXY = "";
    int sizeOfPxyUL = 3;
    int indexOfPxyUL = 0;
    int **PxyUL = new int*[sizeOfPxyUL];
    for (int i = 0; i < indexOfPxUL; i++)
    {
        for (int j = 0; j < indexOfPyUL; j++)
        {
            if (PxUL[i][0] == PyUL[j][0])
            {
                int *Exy = new int[3]{0};
                if (indexOfPUL != 0)
                {
                    for (int k = 0; k < indexOfPUL; k++)
                    {
                        if (PxUL[i][0] == PUL[k][0])
                        {
                            Exy[0] = PxUL[i][0];
                            Exy[1] = PxUL[i][1] + PyUL[j][1] - PUL[k][1];
                            Exy[2] = PyUL[j][2];
                        }
                    }
                    itemXY = itemX.substr(0, itemX.size()-1)+itemX[itemX.size()-1]+itemY[itemY.size()-1];
                } else {
                    Exy[0] = PxUL[i][0];
                    Exy[1] = PxUL[i][1] + PyUL[j][1];
                    Exy[2] = PyUL[j][2];
                    itemXY = itemX + itemY;
                }
                if (indexOfPxyUL >= sizeOfPxyUL)
                {
                    int newSize = sizeOfPxyUL*2;
                    int **temp = new int*[newSize];
                    for (int k = 0; k < sizeOfPxyUL; k++)
                    {
                        temp[k] = PxyUL[k];
                    }
                    delete [] PxyUL;
                    PxyUL = temp;
                    sizeOfPxyUL = newSize;
                }
                PxyUL[indexOfPxyUL++] = Exy;
            }
        }
    }

    UtilityList utilityList(itemXY);
    utilityList.size = sizeOfPxyUL;
    utilityList.index = indexOfPxyUL;
    utilityList.list = PxyUL;
    return utilityList;
}
int ULSumIU(int **UL, int indexOfUL)
{
    int cumulate = 0;
    for (int i = 0; i < indexOfUL; i++)
    {
        cumulate += UL[i][1];
    }
    return cumulate;
}

int ULSumRU(int **UL, int indexOfUL)
{
    int cumulate = 0;
    for (int i = 0; i < indexOfUL; i++)
    {
        cumulate += UL[i][2];
    }
    return cumulate;
}

void HUIMiner(UtilityList *PUL, UtilityList **UL, int minutil, int indexOfUtilityList) {
    for (int i = 0; i < indexOfUtilityList; i++)
    {
        if (ULSumIU(UL[i]->list, UL[i]->index) >= minutil)
        {
            cout << UL[i]->item << ": " << ULSumIU(UL[i]->list, UL[i]->index) << endl;
        }
        if (ULSumIU(UL[i]->list, UL[i]->index) + ULSumRU(UL[i]->list, UL[i]->index) >= minutil)
        {
            int size = 3;
            int index = 0;
            UtilityList **exULs = new UtilityList *[size];
            for (int j = i + 1; j < indexOfUtilityList; j++)
            {
                if (index >= size)
                {
                    int newSize = size*2;
                    UtilityList **temp = new UtilityList *[newSize];
                    for (int k = 0; k < size; k++)
                    {
                        temp[k] = exULs[k];
                    }
                    delete [] exULs;
                    exULs = temp;
                    size = newSize;
                }
                exULs[index++] = new UtilityList(Construct(PUL->list, UL[i]->list, UL[j]->list, PUL->index, UL[i]->index, UL[j]->index, UL[i]->item, UL[j]->item));
            }
            HUIMiner(UL[i], exULs, minutil, index);
        }
    }
}
int main() {
    //initial database
    int threshold = 20;
    int initSize = 2;
    int tidSize = 2;
    int tidIndex = 0;
    int ***db = new int**[tidSize];
    int *itemSize = new int[tidSize];
    int *itemIndex = new int[tidSize];
    for (int i = 0; i < tidSize; i++)
    {
        itemSize[i] = tidSize;
        db[i] = new int*[itemSize[i]];
        for (int j = 0; j < tidSize; j++)
        {
            db[i][j] = new int[2];
        }
        itemIndex[i] = 0;
    }

    int a, b, c;
    while (cin >> a >> b >> c)
    {
        if (a-1 >= tidSize)
        {
            int newSize = tidSize*2;
            int ***tempdb = new int**[newSize];
            int *tempItemSize = new int[newSize];
            int *tempItemIndex = new int[newSize];
            for (int i = 0; i < tidSize; i++)
            {
                tempdb[i] = db[i];
                tempItemSize[i] = itemSize[i];
                tempItemIndex[i] = itemIndex[i];
            }
            for (int i = tidSize; i < newSize; i++)
            {
                tempdb[i] = new int*[tidSize];
                for (int j = 0; j < initSize; j++)
                {
                    tempdb[i][j] = new int[2];
                }
                tempItemSize[i] = initSize;
                tempItemIndex[i] = 0;
            }

            delete [] db;
            delete [] itemSize;
            delete [] itemIndex;
            db = tempdb;
            itemSize = tempItemSize;
            itemIndex = tempItemIndex;
            tidSize = newSize;
        }
        if (itemIndex[a-1] >= itemSize[a-1])
        {
            int newSize = itemSize[a-1]*2;
            int **tempdb = new int*[newSize];
            for (int i = 0; i < itemSize[a-1]; i++)
            {
                tempdb[i] = db[a-1][i];
            }
            for (int i = itemSize[a-1]; i < newSize; i++)
            {
                tempdb[i] = new int[initSize];
            }
            delete [] db[a-1];
            db[a-1] = tempdb;
            itemSize[a-1] = newSize;
        }
        db[a-1][itemIndex[a-1]][1] = c;
        db[a-1][itemIndex[a-1]][0] = b;
        itemIndex[a-1]++;
        tidIndex = max(tidIndex, a);
    }

    //Utility table
    int utilityTable[] = {1, 2, 1, 5, 4, 3, 1};
    int *tu = new int[sizeof(utilityTable)/sizeof(utilityTable[0])]{0};

    for (int i = 0; i < tidIndex; i++)
    {
        for (int j = 0; j < itemIndex[i]; j++)
        {
            tu[i] += utilityTable[db[i][j][0]-1]*db[i][j][1];
        }
    }

    int *twu = new int[sizeof(utilityTable)/sizeof(utilityTable[0])]{0};
    for (int i = 0; i < tidIndex; i++)
    {
        for (int j = 0; j < itemIndex[i]; j++)
        {
            twu[db[i][j][0]-1] += tu[i];
        }
    }

    //construct revised transaction table
    //ascending ordered by twu
    int *tuRevised = new int[sizeof(utilityTable)/sizeof(utilityTable[0])]{0};
    for (int i = 0; i < tidIndex; i++)
    {
        sort(db[i], db[i]+itemIndex[i], [twu](int *a, int *b)
        {
            return twu[a[0]-1] < twu[b[0]-1];
        });
    }

    for (int i = 0; i < tidIndex; i++)
    {
//        cout << (i+1) << " = ";
        for (int j = 0; j < itemIndex[i]; j++)
        {
            if (twu[db[i][j][0]-1] >= threshold)
            {
                tuRevised[i] += db[i][j][1]*utilityTable[db[i][j][0]-1];
//                cout << db[i][j][0] << ":" << db[i][j][1]*utilityTable[db[i][j][0]-1] << ", ";
            }
        }
//        cout << endl;
    }

    int sizeOfTwu = 3;
    int indexOfTwu = 0;
    int *twuOverThreshold = new int[sizeOfTwu];
    int *twuItems = new int[sizeOfTwu];
    string *twuIt = new string[sizeOfTwu];
    for (int i = 0; i < sizeof(utilityTable)/sizeof(utilityTable[0]); i++)
    {
        if (twu[i] >= threshold)
        {
            if (indexOfTwu >= sizeOfTwu)
            {
                int newSize = sizeOfTwu*2;
                int *temp = new int[newSize];
                int *tempItem = new int[newSize];
                string *tempIt = new string[newSize];
                for (int i = 0; i < sizeOfTwu; i++)
                {
                    temp[i] = twuOverThreshold[i];
                    tempItem[i] = twuItems[i];
                    tempIt[i] = twuIt[i];
                }
                delete [] twuOverThreshold;
                delete [] twuItems;
                delete [] twuIt;
                twuOverThreshold = temp;
                twuItems = tempItem;
                twuIt = tempIt;
                sizeOfTwu = newSize;
            }
            int tt = indexOfTwu;
            twuOverThreshold[indexOfTwu] = twu[i];
            char cc = char('a'+i);
            twuIt[indexOfTwu] = char('a'+i);
            twuItems[indexOfTwu++] = i+1;
        }
    }

    UtilityList **utilityList = new UtilityList*[indexOfTwu];
    for (int i = 0; i < indexOfTwu; i++)
    {
        utilityList[i] = new UtilityList(twuIt[i]);
    }

    for (int i = 0; i < tidIndex; i++)
    {
        for (int j = 0; j < itemIndex[i]; j++)
        {
            if (twu[db[i][j][0]-1] >= threshold)
            {
                tuRevised[i] -= db[i][j][1]*utilityTable[db[i][j][0]-1];
//                cout << db[i][j][0] << ", " << i+1 << ", " << db[i][j][1]*utilityTable[db[i][j][0]-1] << ", " << tuRevised[i] << endl;
                utilityList[db[i][j][0]-1]->append(i, db[i][j][1]*utilityTable[db[i][j][0]-1], tuRevised[i]);
                //cout << i << ", " << db[i][j][1]*utilityTable[db[i][j][0]-1] << ", " << tuRevised[i] << endl;
            }
        }
//        cout << endl;
    }

    int *indices = new int[indexOfTwu];
    for (int i = 0; i < indexOfTwu; i++)
    {
        indices[i] = i;
    }

    sort(indices, indices+indexOfTwu, [twu](int a, int b)
    {
        return twu[a] < twu[b];
    });

    UtilityList **tempUtilityList = new UtilityList*[indexOfTwu];
    for (int i = 0; i < indexOfTwu; i++)
    {
        tempUtilityList[i] = utilityList[indices[i]];
    }

    for (int i = 0; i < indexOfTwu; i++)
    {
        utilityList[i] = tempUtilityList[i];
    }

    UtilityList root("");
    HUIMiner(&root, utilityList, threshold, indexOfTwu);

    //release allocation memory
    for (int i = 0; i < tidSize; i++)
    {
        for (int j = 0; j < itemSize[i]; j++)
        {
            delete [] db[i][j];
        }
        delete [] db[i];
    }
    delete [] db;
    delete [] itemSize;
    delete [] itemIndex;

    delete [] tu;
    delete [] twu;
    delete [] tuRevised;
    delete [] twuOverThreshold;
    delete [] twuItems;
    delete [] twuIt;
    for (int i = 0; i < indexOfTwu; i++)
    {
        delete [] utilityList[i];
        delete [] tempUtilityList[i];
    }
    delete [] utilityList;
    delete [] tempUtilityList;
    delete [] indices;
    return 0;
}





