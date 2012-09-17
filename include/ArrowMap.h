//
//  ArrowMap.h
//  ArrowMap
//
//  Created by Sam Johnson on 11/09/12.
//
//

#ifndef __ArrowMap__ArrowMap__
#define __ArrowMap__ArrowMap__

#include <string>
#include <fstream>

class ArrowMap
{
    unsigned int *graph;
    unsigned int graphSize;
    int numNodes;
    
public:
    ArrowMap(const std::string &networkFilePath);
    ~ArrowMap();
    
    unsigned int getNextState(unsigned int state) const;
    unsigned int mapSize() const;
    int networkSize() const;
    //bool isGoE(unsigned int state) const;
    bool isAttractor(unsigned int state) const;
};

#endif /* defined(__ArrowMap__ArrowMap__) */
