#include <dolphin/os/OSCache.h>
#include <dolphin/string.h>

#include "JSystem/JKernel/JKRAram.h"
#include "JSystem/JKernel/JKRArchive.h"
#include "JSystem/JKernel/JKRDecomp.h"
#include "JSystem/JKernel/JKRDvdAramRipper.h"
#include "JSystem/JKernel/JKRDvdRipper.h"
#include "JSystem/JSystem.h"
#include "JSystem/JUtility/JUTAssertion.h"

JKRAramArchive::JKRAramArchive() : JKRArchive() {
}

JKRAramArchive::JKRAramArchive(s32 entryNum, EMountDirection mountDirection) : JKRArchive(entryNum, MOUNT_ARAM) {
    mMountDirection = mountDirection;
    if (!open(entryNum)) {
        return;
    } else {
        mVolumeType = 'RARC';
        mVolumeName = &mStrTable[mDirectories->mOffset];
        sVolumeList.prepend(&mFileLoaderLink);
        mIsMounted = true;
    }
}

JKRAramArchive::~JKRAramArchive() {
    if (mIsMounted == true) {
        if (mArcInfoBlock) {
            SDIFileEntry* fileEntries = mFileEntries;
            for (int i = 0; i < mArcInfoBlock->num_file_entries; i++) {
                if (fileEntries->mData != nullptr) {
                    JKRFreeToHeap(mHeap, fileEntries->mData);
                }
                fileEntries++;
            }
            JKRFreeToHeap(mHeap, mArcInfoBlock);
            mArcInfoBlock = nullptr;
        }

        if (mDvdFile) {
            delete mDvdFile;
        }
        if (mBlock) {
            delete mBlock;
        }

        sVolumeList.remove(&mFileLoaderLink);
        mIsMounted = false;
    }
}

void JKRAramArchive::fixedInit(s32 entryNum, EMountDirection direction) {
    mIsMounted = false;
    mMountDirection = direction;
    mMountMode = 2;
    mMountCount = 1;
    _54 = 2;
    mHeap = JKRGetCurrentHeap();
    mEntryNum = entryNum;

    if (sCurrentVolume)
        return;
    sCurrentVolume = this;
    sCurrentDirID = 0;
}

bool JKRAramArchive::mountFixed(s32 entryNum, EMountDirection direction) {
    if (entryNum < 0)
        return false;

    if (check_mount_already(entryNum))
        return false;

    fixedInit(entryNum, direction);
    if (open(entryNum) == false) {
        return false;
    }

    mVolumeType = 'RARC';
    mVolumeName = &mStrTable[mDirectories->mOffset];
    sVolumeList.prepend(&mFileLoaderLink);
    mIsMounted = true;
    return true;
}

bool JKRAramArchive::mountFixed(const char* path, EMountDirection direction) {
    s32 entrynum = DVDConvertPathToEntrynum((char*)path);
    return mountFixed(entrynum, direction);
}

void JKRAramArchive::unmountFixed() {
    if (sCurrentVolume == this)
        sCurrentVolume = nullptr;

    if (mArcInfoBlock) {
        SDIFileEntry* fileEntries = mFileEntries;
        for (int i = 0; i < mArcInfoBlock->num_file_entries; i++) {
            if (fileEntries->mData != nullptr) {
                JKRFreeToHeap(mHeap, fileEntries->mData);
            }
            fileEntries++;
        }
        JKRFreeToHeap(mHeap, mArcInfoBlock);
        mArcInfoBlock = nullptr;
    }

    if (mDvdFile)
        delete mDvdFile;

    if (mBlock)
        delete mBlock;

    sVolumeList.remove(&mFileLoaderLink);
    mIsMounted = false;
}

#ifdef DEBUG
CW_FORCE_STRINGS(JKRAramArchive_cpp, __FILE__, "isMounted()", "mMountCount == 1")
#endif

bool JKRAramArchive::open(long entryNum) {
    mArcInfoBlock = nullptr;
    mDirectories = nullptr;
    mFileEntries = nullptr;
    mStrTable = nullptr;
    mBlock = nullptr;

    mDvdFile = new (JKRGetSystemHeap(), mMountDirection == MOUNT_DIRECTION_HEAD ? 4 : -4) JKRDvdFile(entryNum);
    if (mDvdFile == nullptr) {
        mMountMode = 0;
        return 0;
    }

    // NOTE: a different struct is used here for sure, unfortunately i can't get
    // any hits on this address, so gonna leave it like this for now
    SArcHeader* mem = (SArcHeader*)JKRAllocFromSysHeap(32, -32);
    if (mem == nullptr) {
        mMountMode = 0;
    } else {
        JKRDvdToMainRam(entryNum, (u8*)mem, EXPAND_SWITCH_DECOMPRESS, 32, nullptr, JKRDvdRipper::ALLOC_DIR_TOP, 0,
                        &mCompression);
        int alignment = mMountDirection == MOUNT_DIRECTION_HEAD ? 32 : -32;
        u32 alignedSize = ALIGN_NEXT(mem->file_data_offset, 32);
        mArcInfoBlock = (SArcDataInfo*)JKRAllocFromHeap(mHeap, alignedSize, alignment);
        if (mArcInfoBlock == nullptr) {
            mMountMode = 0;
        } else {
            JKRDvdToMainRam(entryNum, (u8*)mArcInfoBlock, EXPAND_SWITCH_DECOMPRESS, alignedSize, nullptr,
                            JKRDvdRipper::ALLOC_DIR_TOP, 32, nullptr);

            mDirectories = (SDIDirEntry*)((u8*)mArcInfoBlock + mArcInfoBlock->node_offset);
            mFileEntries = (SDIFileEntry*)((u8*)mArcInfoBlock + mArcInfoBlock->file_entry_offset);
            mStrTable = (const char*)((u8*)mArcInfoBlock + mArcInfoBlock->string_table_offset);

            u32 aramSize = ALIGN_NEXT(mem->file_data_length, 32);
            mBlock = JKRAllocFromAram(aramSize,
                                      mMountDirection == MOUNT_DIRECTION_HEAD ? JKRAramHeap::Head : JKRAramHeap::Tail);
            if (mBlock == nullptr) {
                mMountMode = 0;
            } else {
                JKRDvdToAram(entryNum, mBlock->getAddress(), EXPAND_SWITCH_DECOMPRESS,
                             mem->header_length + mem->file_data_offset, 0);
            }
        }
    }
cleanup:
    if (mem != nullptr) {
        JKRFreeToSysHeap(mem);
    }
    if (mMountMode == 0) {
        JREPORTF(":::[%s: %d] Cannot alloc memory\n", __FILE__,
                 415); // TODO: macro
    }
    return mMountMode != 0;
}

void* JKRAramArchive::fetchResource(SDIFileEntry* fileEntry, u32* pSize) {
    JUT_ASSERT(isMounted());

    u32 sizeRef;
    u8* data;

    if (fileEntry->mData) {
        if (pSize)
            *pSize = fileEntry->mSize;
    } else {
        u32 addres = mBlock->getAddress();
        int compression = JKRConvertAttrToCompressionType(fileEntry->mFlag >> 0x18);
        u32 size =
            fetchResource_subroutine(fileEntry->mDataOffset + addres, fileEntry->mSize, mHeap, compression, &data);
        if (pSize)
            *pSize = size;
        fileEntry->mData = (void*)data;
    }

    return fileEntry->mData;
}

void* JKRAramArchive::fetchResource(void* data, u32 compressedSize, SDIFileEntry* fileEntry, u32* pSize,
                                    JKRExpandSwitch expandSwitch) {
    JUT_ASSERT(isMounted());
    u32 fileSize = fileEntry->mSize;
    if (fileSize > compressedSize) {
        fileSize = compressedSize;
    }
    if (fileEntry->mData) {
        JKRHeap::copyMemory(data, fileEntry->mData, fileSize);
    } else {
        int compression = JKRConvertAttrToCompressionType(fileEntry->mFlag >> 0x18);
        if (expandSwitch != EXPAND_SWITCH_DECOMPRESS)
            compression = 0;

        fileSize = fetchResource_subroutine(fileEntry->mDataOffset + mBlock->getAddress(), fileSize, (u8*)data,
                                            compressedSize, compression);
    }

    if (pSize != nullptr) {
        *pSize = fileSize;
    }
    return data;
}

u32 JKRAramArchive::getAramAddress_Entry(SDIFileEntry* fileEntry) {
    JUT_ASSERT(isMounted());

    if (fileEntry == nullptr) {
        return 0;
    }
    return fileEntry->mDataOffset + mBlock->getAddress();
}

u32 JKRAramArchive::getAramAddress(u32 type, const char* file) {
    SDIFileEntry* entry = findTypeResource(type, file);
    return getAramAddress_Entry(entry);
}

u32 JKRAramArchive::fetchResource_subroutine(u32 srcAram, u32 size, u8* data, u32 expandSize, int compression) {
    JUT_ASSERT((srcAram & 0x1f) == 0);

    u32 sizeRef;

    u32 alignedSize = ALIGN_NEXT(size, 32);
    u32 prevAlignedSize = ALIGN_PREV(expandSize, 32);
    switch (compression) {
        case JKRCOMPRESSION_NONE:
            if (alignedSize > prevAlignedSize) {
                alignedSize = prevAlignedSize;
            }
            JKRAramToMainRam(srcAram, data, alignedSize, EXPAND_SWITCH_DEFAULT, prevAlignedSize, nullptr, -1, &sizeRef);
            return sizeRef;
        case JKRCOMPRESSION_YAY0:
        case JKRCOMPRESSION_YAZ0:
            JKRAramToMainRam(srcAram, data, alignedSize, EXPAND_SWITCH_DECOMPRESS, prevAlignedSize, nullptr, -1,
                             &sizeRef);
            return sizeRef;
        default:
            JPANIC(550, ":::??? bad sequence\n");
            return 0;
    }
}

u32 JKRAramArchive::fetchResource_subroutine(u32 srcAram, u32 size, JKRHeap* heap, int compression, u8** pBuf) {
    u32 resSize;
    u32 alignedSize = ALIGN_NEXT(size, 32);

    u8* buffer;
    switch (compression) {
        case JKRCOMPRESSION_NONE:
            buffer = (u8*)JKRAllocFromHeap(heap, alignedSize, 32);
            JUT_ASSERT(buffer != 0);

            JKRAramToMainRam(srcAram, buffer, alignedSize, EXPAND_SWITCH_DEFAULT, alignedSize, nullptr, -1, nullptr);
            *pBuf = buffer;

            return size;
        case JKRCOMPRESSION_YAY0:
        case JKRCOMPRESSION_YAZ0:
            u8* header = (u8*)JKRAllocFromHeap(heap, 0x20, 0x20);
            JKRAramToMainRam(srcAram, header, 0x20, EXPAND_SWITCH_DEFAULT, 0, nullptr, -1, nullptr);
            u32 expandSize = JKRDecompExpandSize(header);
            JKRFreeToHeap(heap, header);
            expandSize = ALIGN_NEXT(expandSize, 32);
            buffer = (u8*)JKRAllocFromHeap(heap, expandSize, 0x20);
            JUT_ASSERT(buffer);

            JKRAramToMainRam(srcAram, buffer, alignedSize, EXPAND_SWITCH_DECOMPRESS, expandSize, heap, -1, &resSize);
            *pBuf = buffer;
            return resSize;
        default:
            JPANIC(605, ":::??? bad sequence\n");
            return 0;
    }
}
