#include <bits/stdc++.h> 
#include <fstream>

using namespace std; 
  
// driver code 
int main() 
{ 
    // filestream variable file 
    fstream file; 
    string word, filename; 
  
    // filename of the file 
    filename = "master_productions.txt"; 
  
    // opening file 
    file.open(filename.c_str()); 
  
    // extracting words from the file 
    unordered_set<string>st;
    vector <string> x = {"ENUM_START","PROGRAM", "MODULEDECLARATIONS","OTHERMODULES","DRIVERMODULE",
    "MODULEDECLARATION","MODULEDEF","MODULE1","INPUT_PLIST","N1","RET","OUTPUT_PLIST",
    "N2","DATATYPE","TYPE","RANGE","STATEMENTS","STATEMENT","IOSTMT","SIMPLESTMT","DECLARESTMT",
    "CONDITIONALSTMT","ITERATIVESTMT","VAR","WHICHID","ASSIGNMENTSTMT","MODULEREUSESTMT",
    "WHICHSTMT","LVALUEIDSTMT","LVALUEARRSTMT","EXPRESSION","INDEX","OPTIONAL","IDLIST","N3",
    "ARITHMETICEXPR","OP1","TERM","OP2","FACTOR","N4","ARITHMETICORBOOLEANEXPR","ANYTERM",
    "LOGICALOP","RELATIONALOP","CASESTMTS","DEFAULT1","N7","N8","N9","VALUE",  
    "RANGE_ARRAYS","N5","BOOLCONSTT","VAR_ID_NUM","U","NEW_NT","UNARY_OP",                          //Non-Terminals
    "$",       //sentinel
    "INTEGER","REAL","BOOLEAN","OF","ARRAY","MODULE","START","END","DECLARE","DRIVER","UNION","TAGGED","RECORD","GET_VALUE","PRINT",
    "USE","WITH","PARAMETERS","TAKES","INPUT","RETURNS","AND","OR","FOR","IN","SWITCH","CASE","BREAK",
    "WHILE","PLUS","MINUS","MUL","DIV","LT","LE","GE","GT","EQ","NE","DEF","ENDDEF","DRIVERDEF","DRIVERENDDEF",
    "COLON","RANGEOP","SEMICOL","COMMA","ASSIGNOP","SQBO","SQBC","BO","BC","COMMENTMARK","ID","NUM","RNUM","DEFAULT",
    "FALSE1","TRUE1","EPS","ENUM_END"}; 

    ofstream ofs("symbollist.txt");
    while (file >> word) 
    { 
        // displaying content 
        st.insert(word);
    } 
    for(int i=0;i<x.size();i++)
    {
        if(st.find(x[i])==st.end())
        {
            cout << x[i] << " ";
        }
    }
    cout << endl << endl;
    vector<string> v(st.begin(),st.end());
    sort(v.begin(),v.end());
    for(int i=0;i<v.size();i++)
    {
        cout << v[i] << endl;
        ofs << v[i] << endl;

    }
    cout << v.size() << " " << x.size() << endl;                                                                         //Terminals


  
    return 0; 
} 