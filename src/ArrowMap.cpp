//
//  ArrowMap.cpp
//  ArrowMap
//
//  Created by Sam Johnson on 11/09/12.
//
//

#include <iostream>
#include "ArrowMap.h"

ArrowMap::ArrowMap(const std::string &networkFilePath)
{
    //Unpack information from file
    numNodes = 0;
    unsigned int *ancLengths;
    unsigned int **structure;
    unsigned int *lutLengths;
    bool **nodeLUTs;
    char buf[2]; 
    
    std::ifstream networkFile;
    networkFile.open(networkFilePath.c_str(), std::ios::binary);
    
    if (!networkFile.is_open())
        throw "File not found.";
    
    networkFile.read(buf, 2);
    numNodes = buf[0] << 8 | buf[1];
    ancLengths = new unsigned int[numNodes];
    structure = new unsigned int*[numNodes];
    nodeLUTs = new bool*[numNodes];
    lutLengths = new unsigned int[numNodes];
    for (int i = 0; i < numNodes; i++)
    {
        networkFile.read(buf, 1);
        ancLengths[i] = buf[0];
        //networkFile.read((char*)&ancLengths[i], 1);
        structure[i] = new unsigned int[ancLengths[i]];
        for (int j = 0; j < ancLengths[i]; j++)
        {
            networkFile.read(buf, 2);
            structure[i][j] = buf[0] << 8 | buf[1];
        }
        
        lutLengths[i] = 1 << ancLengths[i];
        nodeLUTs[i] = new bool[lutLengths[i]];
        for (int j = 0; j < lutLengths[i]; j++)
        {
            if (j % 8 == 0)
                networkFile.read(buf, 1);
            
            nodeLUTs[i][j] = buf[0] >> (7-j%8) & 1;
        }
    }
    
    /*std::cout << numNodes << "\n\n";
    for (int i = 0; i < numNodes; i++)
    {
        std::cout << ancLengths[i] << "\n";
        for (int j = 0; j < ancLengths[i]; j++)
            std::cout << structure[i][j] << "\n";
        std::cout << lutLengths[i] << "\n";
        for (int j = 0; j < lutLengths[i]; j++)
            std::cout << nodeLUTs[i][j] << " ";
        std::cout << "\n\n";
    }*/
    
    //Compute entire state space graph
    bool *states = new bool[numNodes];
    graphSize = 1 << numNodes;
    graph = new unsigned int[graphSize];
    for (unsigned int i = 0; i < graphSize; i++)
    {
        for (int j = 0; j < numNodes; j++)
            states[numNodes - j - 1] = i >> j & 1;
        
        graph[i] = 0;
        for (int j = 0; j < numNodes; j++)
        {
            int n = 0;
            for (int k = 0; k < ancLengths[j]; k++)
                n |= states[structure[j][k]] << k;
            graph[i] <<= 1;
            graph[i] |= nodeLUTs[j][n];
        }
    }
    
    delete [] states;
    for (int i = 0; i < numNodes; i++)
    {
        delete [] structure[i];
        delete [] nodeLUTs[i];
    }
    delete [] structure;
    delete [] nodeLUTs;
    delete [] ancLengths;
    delete [] lutLengths;
}

ArrowMap::~ArrowMap()
{
    delete [] graph;
}

unsigned int ArrowMap::getNextState(unsigned int state) const
{
    return graph[state];
}

unsigned int ArrowMap::mapSize() const
{
    return graphSize;
}

int ArrowMap::networkSize() const
{
    return numNodes;
}

bool ArrowMap::isAttractor(unsigned int state) const
{
    return false;
}