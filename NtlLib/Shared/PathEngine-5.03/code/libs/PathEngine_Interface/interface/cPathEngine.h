//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef C_PATHENGINE_INCLUDED
#define C_PATHENGINE_INCLUDED

#include "i_pathengine.h"
#include "common/Containers/ReplacementList.h"
#include "common/Handle.h"
#include "libs/PathEngine_Interface/interface/cMesh.h"
#include "libs/PathEngine_Interface/interface/cShape.h"
#include "libs/PathEngine_Interface/interface/cContentChunk.h"
#include "libs/Mesh3D/interface/tMesh_3D_Header.h"

template <class T> class cPool;
class cPath;
class cCollisionInfo;
class cPreprocessShapes;
class cAStarHeapPool;
class cCollisionShape;
class cRandomSequence;
class iRandomSequence;
class cMutex;
template <class T> class cPool;

class cPathEngine : public iPathEngine
{
    cPreprocessShapes* _preprocessShapes;
    cAStarHeapPool* _heapPool;

    cList_NoSize<cShape*> _heldShapes;
    cMutex* _heldShapesMutex;

    cList_NoSize<cMesh*> _heldMeshes;
    cMutex* _heldMeshesMutex;

    cList_NoSize<cContentChunk*> _heldContentChunks;
    cMutex* _heldContentChunksMutex;

    cRandomSequence* _randomSequence;
    cMutex* _randomSequenceMutex;

    cMutex* _generalMutex;
    long _pathReserveSize;

    cPool<cPath>* _pathPool;
    cPool<cCollisionInfo>* _collisionInfoPool;

public:

    // for construction in Singletons.cpp
    //...... make these private again
    cPathEngine();
    void stage2Initialisation();
    ~cPathEngine();

// iPathEngine

    const char *const* getVersionAttributes() const;
    long getInterfaceMajorVersion() const;
    long getInterfaceMinorVersion() const;
    void getReleaseNumbers(long& majorReleaseNumber, long& minorReleaseNumber, long& internalReleaseNumber) const;
    void setErrorHandler(iErrorHandler* newHandler);
    iErrorHandler* getErrorHandler();
    void setPathPoolParameters(long pathsPooled, long pointsPerPath);
    bool shapeIsValid(long numberOfPoints, const long* coordinateData);
    iShape* newShape(long numberOfPoints, const long* coordinateData);
    iMesh* loadMeshFromBuffer(const char* format, const char* dataBuffer, unsigned long bufferSize, const char *const* options);
    iMesh* buildMeshFromContent(const iFaceVertexMesh* const* geometryObjectPointers, long numberOfGeometryObjects, const char *const* options);
    void saveContentData(const iFaceVertexMesh* const* geometryObjectPointers, long numberOfGeometryObjects, const char* format, iOutputStream* os);
    unsigned long totalMemoryAllocated();
    unsigned long maximumMemoryAllocated();
    void resetMaximumMemoryAllocated();
    void setRandomSeed(unsigned long value);
    bool collisionPreprocessVersionIsCompatible(const char* dataBuffer, unsigned long bufferSize);
    bool pathfindPreprocessVersionIsCompatible(const char* dataBuffer, unsigned long bufferSize);
    void deleteAllObjects();
    iContentChunk* newContentChunk(const iFaceVertexMesh* ground, const iAnchorsAndPinnedShapes* anchorsAndShapes, const char *const* attributes);
    iContentChunk* loadContentChunk(const char* dataBuffer, unsigned long bufferSize);
    void operator delete(void* p) {::operator delete(p);}

//=========================

    void
    saveContentData(
            const iFaceVertexMesh* const* geometryObjectPointers, long numberOfGeometryObjects,
            const iAnchorsAndPinnedShapes* anchorsAndShapes,
            const char* format,
            iOutputStream* os
            );

    iMesh* constructMeshFrom3D(
            tMesh_3D& baseMesh,
            std::vector<long>* surfaceTypes,
            std::vector<long>* sectionIDs,
            std::vector<long>* userData,
            const char *const* options
            );

    long allocateIndexForPreprocessShape(const cCollisionShape& shape);
    long numberOfPreprocessShapes() const;
    const cCollisionShape& refPreprocessShape(long index) const;
    void getPreprocessShapeBounds(long index, tPoint& boundsMin, tPoint& boundsMax) const;

    void lockHeldShapes() const;
    void unlockHeldShapes() const;
    void lockHeldMeshes() const;
    void unlockHeldMeshes() const;
    void lockHeldContentChunks() const;
    void unlockHeldContentChunks() const;

    long getRandomSeed() const;
    void setRandomSeed_Long(long value);
    long random15();
    long random31();
    iRandomSequence& lockRandomSequence();
    void releaseRandomSequence();

    cAStarHeapPool& refHeapPool()
    {
        return *_heapPool;
    }
    long getPathReserveSize();
    cPool<cPath>& refPathPool()
    {
        return *_pathPool;
    }

    cPool<cCollisionInfo>& refCollisionInfoPool()
    {
        return *_collisionInfoPool;
    }

    iShape* newShape_Internal(const cCollisionShape& cs);

};

inline cPathEngine* FromI(iPathEngine* ptr)
{
    return static_cast<cPathEngine*>(ptr);
}

#endif
