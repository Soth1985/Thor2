/**** Decompose.h - Basic declarations ****/
#ifndef _H_Decompose
#define _H_Decompose
typedef struct {float x, y, z, w;} QuatXX; /* Quaternion */
enum QuatPart {X, Y, Z, W};
typedef QuatXX HVect; /* Homogeneous 3D vector */
typedef float HMatrix[4][4]; /* Right-handed, for column vectors */
typedef struct {
    HVect t;	/* Translation components */
    QuatXX  q;	/* Essential rotation	  */
    QuatXX  u;	/* Stretch rotation	  */
    HVect k;	/* Stretch factors	  */
    float f;	/* Sign of determinant	  */
} AffineParts;
float polar_decomp(HMatrix M, HMatrix Q, HMatrix S);
QuatXX spect_decomp(HMatrix S, HMatrix U);
QuatXX snuggle(QuatXX q, HVect *k);
void decomp_affine(HMatrix A, AffineParts *parts);
void invert_affine(AffineParts *parts, AffineParts *inverse);
QuatXX Qt_FromMatrix(HMatrix mat);
#endif

