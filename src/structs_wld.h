
#ifndef _ZEQ_STRUCTS_WLD_H
#define _ZEQ_STRUCTS_WLD_H

#include "types.h"

namespace WLD_Structs
{
#pragma pack(1)

	struct FragHeader
	{
		uint32 len;
		uint32 type;
		int nameref;
		static const uint32 SIZE = 8; //nameref isn't considered part of the frag header, but it's always there
	};

	struct BoneAssignment
	{
		int16 count;
		int16 index;
	};

	struct Frag36 : public FragHeader //mesh fragment
	{
		uint32 flag;
		int texture_list_ref;
		int anim_vert_ref;
		uint32 unknownA[2];
		float x, y, z;
		float rotation[3];
		float unusedA[7];
		uint16 vert_count;
		uint16 uv_count;
		uint16 normal_count;
		uint16 color_count;
		uint16 poly_count;
		uint16 bone_assignment_count;
		uint16 poly_texture_count;
		uint16 vert_texture_count;
		uint16 size9;
		uint16 scale;
	};

	struct Frag31 : public FragHeader //texture list fragment
	{
		uint32 flag;
		uint32 ref_count;

		int* getRefList()
		{
			return (int*)((byte*)this + sizeof(Frag31));
		}
	};

	struct Frag30 : public FragHeader
	{
		enum VisibilityFlags
		{
			//these aren't "right", but they work mostly
			MASKED = 0x12,
			SEMI_TRANSPARENT = 0x5
		};
		uint32 flag;
		uint32 visibility_flag;
		uint32 unknown[3];
		int ref;
	};

	struct Frag2D : public FragHeader //mesh reference fragment
	{
		int ref;
		uint32 flag;
	};

	struct Position
	{
		float x, y, z;
	};

	struct Frag15 : public FragHeader //object location fragment
	{
		int ref1;
		uint32 flag;
		int ref2;
		Position pos;
		Position rot;
		Position scale;
		int ref3;
		uint32 ref3_param;
	};

	struct Frag14 : public FragHeader //model fragment
	{
		uint32 flag;
		int ref1;
		int size[2];
		int ref2;
		//followed by a list of refs
		int* getRefList()
		{
			byte* data = (byte*)this + sizeof(Frag14);
			//skip optional fields if they exist
			if (flag & (1 << 0))
				data += 4;
			if (flag & (1 << 1))
				data += 4;
			//skipping of various sizes
			for (int i = 0; i < size[0]; ++i)
				data += *(int*)data * 8 + sizeof(int);
			return (int*)data;
		}
	};

	struct Frag13 : public FragHeader
	{
		int ref;
		uint32 flag;
		uint32 param;
	};

	struct Frag12Entry
	{
		int16 rotDenom;
		int16 rotX, rotY, rotZ;
		int16 shiftX, shiftY, shiftZ;
		int16 shiftDenom;
	};

	struct Frag12 : public FragHeader //skeleton bone track fragment
	{
		uint32 flag;
		uint32 count;
		Frag12Entry entry[1];
	};

	struct Frag11 : public FragHeader //skeleton ref fragment
	{
		int ref;
		uint32 flag;
	};

	struct Frag10Bone
	{
		int nameref;
		uint32 flag;
		int ref1;
		int ref2;
		int size;

		int* getIndexList()
		{
			return (int*)((byte*)this + sizeof(Frag10Bone));
		}

		Frag10Bone* getNext()
		{
			return (Frag10Bone*)((byte*)this + sizeof(Frag10Bone) + size * sizeof(int));
		}
	};

	struct Frag10 : public FragHeader //skeleton set fragment
	{
		uint32 flag;
		int num_bones;
		int ref;

		Frag10Bone* getBoneList()
		{
			byte* data = (byte*)this + sizeof(Frag10);
			//skip optional fields if they exist
			if (flag & (1 << 0))
				data += 12;
			if (flag & (1 << 1))
				data += 4;
			return (Frag10Bone*)data;
		}

		int* getRefList(int& out_num)
		{
			byte* data = (byte*)this + sizeof(Frag10);
			//skip optional fields if they exist
			if (flag & (1 << 0))
				data += 12;
			if (flag & (1 << 1))
				data += 4;
			//skipping of various sizes
			for (int i = 0; i < num_bones; ++i)
			{
				data += 16;
				int count = *(int*)data;
				data += count * 4 + sizeof(int);
			}
			out_num = *(int*)data;
			data += sizeof(int);
			return (int*)data;
		}

		bool hasRefList()
		{
			return (flag & (1 << 9)) != 0;
		}
	};

	struct Frag05 : public FragHeader //material fragment
	{
		int ref;
		uint32 flag;
	};

	struct Frag04 : public FragHeader //texture list fragment
	{
		uint32 flag;
		int count;
		int ref;

		bool isAnimated()
		{
			return count > 1;
		}
	};

	struct Frag04Animated : public FragHeader
	{
		uint32 flag;
		int count;
		uint32 milliseconds;
		//refs follow
		int* getRefList()
		{
			return (int*)((byte*)this + sizeof(Frag04Animated));
		}
	};

	struct Frag03 : public FragHeader //texture name fragment
	{
		int count; //always 0/1
		uint16 string_len;
		byte string[2];
	};

	struct RawVertex
	{
		int16 x, y, z;
	};

	struct RawUV16
	{
		int16 u, v;
	};

	struct RawUV32
	{
		float u, v;
	};

	struct RawNormal
	{
		int8 i, j, k;
	};

	struct RawTriangle
	{
		enum Flags
		{
			PERMEABLE = 0x10 //i.e. not subject to collision
		};
		uint16 flag;
		uint16 index[3];
	};

	struct RawTextureEntry
	{
		uint16 count;
		uint16 index;
	};

#pragma pack()
}

#endif
