#include<bits/stdc++.h>
using namespace std;
struct Node
{
    int courseNo;
    int studentsTaken;

};
int main()
{

    string testCaseName = "Toronto/car-f-92";
    string studentFile = testCaseName+".stu";
    string courseFile = testCaseName+".crs";
    
    vector<vector<int>> studentCourseList;
    vector<Node> courseFrequency;

    function<void()> fileInput = [&] () -> void
    {
        ifstream file(studentFile);
        string str;
        int i=0;
        while (getline(file, str)) 
        {
            vector<int> currentStudent;
            stringstream ss;
            ss<<str;
            while(!ss.eof())
            {
                int t;
                ss>>t;
                currentStudent.push_back(t);
            }
            studentCourseList.push_back(currentStudent);
            
        }
        file.close();
        file.open(courseFile);

        
        while (getline(file, str)) 
        {
            
            stringstream ss(str);
            int t1,t2;
            while(!ss.eof())
            {
                ss>>t1;
                ss>>t2;
                Node t3;
                t3.courseNo = t1;
                t3.studentsTaken = t2;
                courseFrequency.push_back(t3);
            }
            
            
            
        }
    };
    fileInput();
    
    int totalNode = courseFrequency.size()+1;  // 1 indexing so 0 is dummy node
    vector<vector<int>> adjMatrix;

    function<void()> adjacencyMatrixCreation = [&] () -> void
    {
        for(int i=0;i<totalNode;i++)
        {
            vector<int> t;
            for(int j=0;j<totalNode;j++)
            {
                t.push_back(0);
            }
            adjMatrix.push_back(t);
        }

        for(auto &t : studentCourseList)
        {
            for(int i=0;i<t.size();i++)
            {
                int leftVertex = t[i];
                for(int j=i+1;j<t.size();j++)
                {
                    int rightVertex = t[j];
                    adjMatrix[leftVertex][rightVertex]++;
                    adjMatrix[rightVertex][leftVertex]++;
                }
            }
        }
    };
    adjacencyMatrixCreation();
    

    vector<int> colorAssigned;
    int totalColorSoFar = 0;
    

    function<void()> largestEnrollment = [&] () -> void
    {
        for(int i=0;i<totalNode;i++)
        {
            colorAssigned.push_back(-5);
        }
        totalColorSoFar = 0;
        vector<int> colorFrequency;
        function<bool(Node,Node)> compareNode = [&] (Node n1, Node n2) -> bool
        {
            return n1.studentsTaken<n2.studentsTaken;

        };

        sort(courseFrequency.begin(),courseFrequency.end(),compareNode);
        reverse(courseFrequency.begin(),courseFrequency.end());

        for(auto &t : courseFrequency)
        {
            int vertex = t.courseNo;
            unordered_set <int> s;
            for(int i=0;i<totalColorSoFar;i++)
            {
                
                s.insert(i);
            }
            for( int i=0;i<adjMatrix[vertex].size();i++)
            {
                int t1 = adjMatrix[vertex][i];
                if(t1>0)
                {
                    int color = colorAssigned[i];
                    if(color>=0)
                    {
                        s.erase(color);
                    }
                }
            }
            int colorContender = -1;
            int contenderFrequency = INT_MAX;

            for( auto it = s.begin(); it!=s.end() ; it++)
            {
                int color = *it;
                int freq = colorFrequency[color];
                if(freq<=contenderFrequency)
                {
                    colorContender = color;
                    contenderFrequency = freq;
                }
            }

            if(colorContender == -1)
            {
                colorContender = totalColorSoFar;
                totalColorSoFar++;
                contenderFrequency = 1;
                colorFrequency.push_back(contenderFrequency);
            }
            else
            {
                colorFrequency[colorContender]++;
            }
            colorAssigned[vertex] = colorContender;
            


        }
        // cout<<totalColorSoFar<<endl;
        // for(i=0;i<colorFrequency.size();i++)
        // {
        //     cout<<i<<" "<<colorFrequency[i]<<endl;
        // }

    };

    function<void()> DSatur = [&] () -> void
    {
        for(int i=0;i<totalNode;i++)
        {
            colorAssigned.push_back(-5);
        }
        totalColorSoFar = 0;
        unordered_map<int,int> degree;
        for(int i=0;i<adjMatrix.size();i++)
        {
            int count = 0;
            for(int j=0;j<adjMatrix[i].size();j++)
            {
                if(adjMatrix[i][j]>0)
                {
                    count++;
                }
            }
            degree[i] = count;
        }
        unordered_map<int,int> sat;
        for(int i=0;i<degree.size();i++)
        {
            sat[i] = 0;
        }

        while(!sat.empty())
        {
            int cs = -5;  // current saturation
            int cd = -5;  // current degree
            int cv = -5;  // current vertex
            for(auto &t : sat)
            {
                int key = t.first;
                int val = t.second;
                if(cs<val)
                {
                    cv = key;
                    cd = degree[key];
                    cs = val;
                }
                else if(cs==val)
                {
                    if(cd<degree[key])
                    {
                        cv = key;
                        cd = degree[key];
                        cs = val;
                    }
                }
                
            }
            unordered_set<int> s;
            for(int i=0;i<totalColorSoFar;i++)
            {
                s.insert(i);
            }
            for(int i=0;i<adjMatrix[cv].size();i++)
            {
                if(adjMatrix[cv][i]>0)
                {
                    if(colorAssigned[i]>=0)
                    {
                        s.erase(colorAssigned[i]);
                    }
                }
            }
            int vColor;
            if(s.empty())
            {
                vColor = totalColorSoFar;
                totalColorSoFar++;
            }
            else
            {
                vColor = *(s.begin());
            }
            colorAssigned[cv] = vColor;
            for(int i=0;i<adjMatrix[cv].size();i++)
            {
                if(adjMatrix[cv][i]>0)
                {
                    if(sat.find(i)!=sat.end())
                    {
                        sat[i]++;
                    }
                }
            }
            sat.erase(cv);
            


        }
        //cout<<totalColorSoFar<<endl;

    };
    DSatur();
    //largestEnrollment();
    vector<pair<int,int>> blameValue(totalNode);
    function<float(void)> countPenalty = [&] () -> float 
    {
        for(int i=0;i<blameValue.size();i++)
        {
            blameValue[i].first = i;
            blameValue[i].second = 0;
        }
        float penalty = 0;
        unordered_map<int,int> penaltyAmount;
        penaltyAmount[1] = 16;
        penaltyAmount[2] = 8;
        penaltyAmount[3] = 4;
        penaltyAmount[4] = 2;
        penaltyAmount[5] = 1;
        for(auto &t : studentCourseList)
        {
            vector<int> slots;
            for(auto &t1 : t)
            {
                slots.push_back(colorAssigned[t1]);
            }
            sort(slots.begin(),slots.end());
            int penaltyForAStudent = 0;
            for(int i=1;i<slots.size();i++)
            {
                int dif = slots[i] - slots[i-1];
                int left = i-1;
                int right = i;
                
                if(dif<=5)
                {
                    
                    penaltyForAStudent += penaltyAmount[dif];
                    blameValue[left].second += penaltyAmount[dif];
                    blameValue[right].second += penaltyAmount[dif];
                    
                }
            }
            //cout<<penaltyForAStudent<<endl;
            penalty += penaltyForAStudent;

        }
        float t = studentCourseList.size();
        penalty = penalty/t;// avg penalty
        //cout<<"penalty: "<<penalty<<endl;
        return penalty;
        
    };

    float avgPenalty = countPenalty();
    
    cout<<"total color needed: "<<totalColorSoFar<<endl;
    cout<<"penalty: "<<avgPenalty<<endl;
    
    float currentPenalty = avgPenalty;

    function<void()> kempeChainInterchange = [&] () -> void 
    
    {
        for(int i=0;i<courseFrequency.size();i++)
        {
            //cout<<i<<endl;
            int vertex1 = courseFrequency[i].courseNo;
            int c1 = colorAssigned[vertex1];
            for(int j=i+1;j<courseFrequency.size();j++)
            {
                
                int vertex2 = courseFrequency[j].courseNo;
                int c2 = colorAssigned[vertex2];
                if(adjMatrix[vertex1][vertex2]<=0)
                {
                    continue;
                }
                
                bool *visited = new bool[totalNode];
                for(int z=0;z<totalNode;z++)
                {
                    visited[z] = false;
                }
                

                vector<int> sameColorVertices;

                stack<int> s;
                s.push(vertex2);
                s.push(vertex1);
                sameColorVertices.push_back(vertex1);
                sameColorVertices.push_back(vertex2);
                visited[vertex1] = true;
                visited[vertex2] = true;
                while(!s.empty())
                {
                    int t = s.top();
                    s.pop();
                    for(int i1=0;i1<adjMatrix[t].size();i1++)
                    {
                        if(adjMatrix[t][i1]>0 && visited[i1]==false)
                        {
                            if(colorAssigned[i1]==c1 || colorAssigned[i1]==c2)
                            {
                                s.push(i1);
                                sameColorVertices.push_back(i1);
                                visited[i1] = true;

                            }
                        }
                    }
                }

                for(auto &t : sameColorVertices)
                {
                    if(colorAssigned[t]==c1)
                    {
                        colorAssigned[t] = c2;
                    }
                    else
                    {
                        colorAssigned[t] = c1;
                    }
                }

                float newPenalty = countPenalty();
                //cout<<"newPenalty: "<<newPenalty<<endl;
                if(newPenalty>currentPenalty)
                {
                    for(auto &t : sameColorVertices)
                    {
                        if(colorAssigned[t]==c1)
                        {
                            colorAssigned[t] = c2;
                        }
                        else
                        {
                            colorAssigned[t] = c1;
                        }
                    }
                }
                else
                {
                    currentPenalty = newPenalty;
                }
                



                
            }
        }
    };
    
    function<void()> kempeChainInterchangeV2 = [&] () -> void 
    {
        for(int i=0;i<courseFrequency.size();i++)
        {
            cout<<i<<endl;
            int vertex1 = courseFrequency[i].courseNo;
            int c1 = colorAssigned[vertex1];
            for(int j=0;j<colorAssigned.size();j++)
            {
                if(c1==j)
                {
                    continue;
                }

                for(int k=0;k<adjMatrix[i].size();k++)
                {
                    
                    if(adjMatrix[i][k]>0 && colorAssigned[k]==j)
                    {

                
                        int vertex2 = k;
                        int c2 = colorAssigned[vertex2];
                
                
                        bool *visited = new bool[totalNode];
                        for(int z=0;z<totalNode;z++)
                        {
                            visited[z] = false;
                        }
                

                        vector<int> sameColorVertices;

                        stack<int> s;
                        s.push(vertex2);
                        s.push(vertex1);
                        sameColorVertices.push_back(vertex1);
                        sameColorVertices.push_back(vertex2);
                        visited[vertex1] = true;
                        visited[vertex2] = true;
                        while(!s.empty())
                        {
                            int t = s.top();
                            s.pop();
                            for(int i1=0;i1<adjMatrix[t].size();i1++)
                            {
                                if(adjMatrix[t][i1]>0 && visited[i1]==false)
                                {
                                    if(colorAssigned[i1]==c1 || colorAssigned[i1]==c2)
                                    {
                                        s.push(i1);
                                        sameColorVertices.push_back(i1);
                                        visited[i1] = true;

                                    }
                                }
                            }
                        }

                        for(auto &t : sameColorVertices)
                        {
                            if(colorAssigned[t]==c1)
                            {
                                colorAssigned[t] = c2;
                            }
                            else
                            {
                                colorAssigned[t] = c1;
                            }
                        }

                        float newPenalty = countPenalty();
                        //cout<<"newPenalty: "<<newPenalty<<endl;
                        if(newPenalty>currentPenalty)
                        {
                            for(auto &t : sameColorVertices)
                            {
                                if(colorAssigned[t]==c1)
                                {
                                    colorAssigned[t] = c2;
                                }
                                else
                                {
                                    colorAssigned[t] = c1;
                                }
                            }
                        }
                        else
                        {
                            currentPenalty = newPenalty;
                        }
                        


                    }
                }
            }
        }
    };

    function<void()> kempeChainInterchangeV3 = [&] () -> void 
    {
        vector<unordered_set<int>> neighbourColors(totalNode);
        unordered_set<int> nonVisitedVertex;
        for(int i=0;i<adjMatrix.size();i++)
        {
            nonVisitedVertex.insert(i);
            unordered_set<int> &r = neighbourColors[i];
            for(int j=0;j<adjMatrix[i].size();j++)
            {
                if(adjMatrix[i][j]>0)
                {
                    r.insert(colorAssigned[j]);
                }
            }
        }
        int iterationNumber = 0;
        while(!nonVisitedVertex.empty()&&iterationNumber<100)
        {
            iterationNumber++;
            //cout<<iterationNumber<<endl;
            
            int vertex1 = *nonVisitedVertex.begin();
            nonVisitedVertex.erase(vertex1);
            int c1 = colorAssigned[vertex1];
            unordered_set<int> &r = neighbourColors[vertex1];
            
            for(auto c2: r)
            {
                vector<int> chainVertex;
                bool *visited = new bool[totalNode];
                for(int i=0;i<totalNode;i++)
                {
                    visited[i] = false;
                }
                stack<int> s;
                s.push(vertex1);
                chainVertex.push_back(vertex1);
                visited[vertex1] = true;
                while(!s.empty())
                {
                    int cv = s.top();
                    s.pop();
                    for(int i=0;i<totalNode;i++)
                    {
                        if(adjMatrix[cv][i]>0 && visited[i]==false)
                        {
                            if(colorAssigned[i]==c1 || colorAssigned[i]==c2)
                            {
                                visited[i] = true;
                                chainVertex.push_back(i);
                                s.push(i);
                            }
                        }
                    }

                }

                for(auto &t : chainVertex)
                {
                    if(colorAssigned[t]==c1)
                    {
                        colorAssigned[t] = c2;
                    }
                    else
                    {
                        colorAssigned[t] = c1;
                    }
                }

                float newPenalty = countPenalty();
                //cout<<"newPenalty: "<<newPenalty<<endl;
                if(newPenalty>currentPenalty)
                {
                    for(auto &t : chainVertex)
                    {
                        if(colorAssigned[t]==c1)
                        {
                            colorAssigned[t] = c2;
                        }
                        else
                        {
                            colorAssigned[t] = c1;
                        }
                    }
                }
                else
                {
                    currentPenalty = newPenalty;
                    for(auto &t: chainVertex)
                    {
                        nonVisitedVertex.erase(t);
                    }
                    break;


                }


            }
        }
        
    };

    kempeChainInterchangeV3();
    cout<<"after kempe chain interchange new penalty: "<<currentPenalty<<endl;
    //cout<<"bye1"<<endl;
    
    

    function<void()> DSaturSWO = [&] () -> void
    {
        int targetNode = totalColorSoFar / 5;
        function<bool(pair<int,int>,pair<int,int>)> comparePair = [&] (pair<int,int> n1, pair<int,int> n2) -> bool
        {
            return n1.second<n2.second;
        };
    // for(int i=0;i<blameValue.size();i++)
    // {
    //     cout<<i<<" "<<blameValue[i].second<<endl;
    // }
        sort(blameValue.begin(),blameValue.end(),comparePair);
        reverse(blameValue.begin(),blameValue.end());
        vector<int> priority;
        for(int i=0;i<targetNode;i++)
        {
            //cout<<blameValue[i].first<<" "<<blameValue[i].second<<endl;
            priority.push_back(blameValue[i].first);

        }
        for(int i=0;i<totalNode;i++)
        {
            colorAssigned.push_back(-5);
        }
        int currentColor = 0;
        for(int i=0;i<priority.size();i++)
        {
            int courseNumber = priority[i];
            colorAssigned[courseNumber] = currentColor;
            currentColor += targetNode;
        }
        
        unordered_map<int,int> degree;
        for(int i=0;i<adjMatrix.size();i++)
        {
            int count = 0;
            for(int j=0;j<adjMatrix[i].size();j++)
            {
                if(adjMatrix[i][j]>0)
                {
                    count++;
                }
            }
            degree[i] = count;
        }
        unordered_set<int> alreadyColored;
        
        unordered_map<int,int> sat;
        for(int i=0;i<degree.size();i++)
        {
            
            sat[i] = 0;
            
            
        }
        for(int i=0;i<targetNode;i++)
        {
            int ver = priority[i];
            sat.erase(ver);
            
            for(int j=0;j<adjMatrix[ver].size();j++)
            {
                if(adjMatrix[ver][j]>0)
                {
                    sat[j]++;
                }
            }
        }

        while(!sat.empty())
        {
            int cs = -5;  // current saturation
            int cd = -5;  // current degree
            int cv = -5;  // current vertex
            for(auto &t : sat)
            {
                int key = t.first;
                int val = t.second;
                if(cs<val)
                {
                    cv = key;
                    cd = degree[key];
                    cs = val;
                }
                else if(cs==val)
                {
                    if(cd<degree[key])
                    {
                        cv = key;
                        cd = degree[key];
                        cs = val;
                    }
                }
                
            }
            unordered_set<int> s;
            for(int i=0;i<totalColorSoFar;i++)
            {
                s.insert(i);
            }
            for(int i=0;i<adjMatrix[cv].size();i++)
            {
                if(adjMatrix[cv][i]>0)
                {
                    if(colorAssigned[i]>=0)
                    {
                        s.erase(colorAssigned[i]);
                    }
                }
            }
            int vColor;
            if(s.empty())
            {
                vColor = totalColorSoFar;
                totalColorSoFar++;
            }
            else
            {
                vColor = *(s.begin());
            }
            colorAssigned[cv] = vColor;
            for(int i=0;i<adjMatrix[cv].size();i++)
            {
                if(adjMatrix[cv][i]>0)
                {
                    if(sat.find(i)!=sat.end())
                    {
                        sat[i]++;
                    }
                }
            }
            sat.erase(cv);
            


        }
        //cout<<totalColorSoFar<<endl;

    };
    DSaturSWO();        
    kempeChainInterchangeV3();


    float currentPenaltyAfterSwo = countPenalty();
    cout<<"after desaturSWO"<<endl;
    cout<<"total color needed: "<<totalColorSoFar<<endl;
    cout<<"new penalty: "<<currentPenaltyAfterSwo<<endl;
    cout<<"bye"<<endl;
    
    
    return 0;
}