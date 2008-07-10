#ifndef __PlaneSweepStereoGPU_h__
#define __PlaneSweepStereoGPU_h__

#include <vnl/vnl_float_3x3.h>
#include <vnl/vnl_float_3.h>

#include "PlaneSweepStereo.h"
#include "../opengl/opengl.h"



template<class T>
vnl_float_3x3 vnl_float_3x3_from_x( const T &x )
{
	vnl_float_3x3 res;
	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
			res(i,j) = x(i,j);
	return res;
}
template<class T>
vnl_float_3 vnl_float_3_from_x( const T &x )
{
	vnl_float_3 res;
	for(int i=0;i<3;i++)
		res(i) = x(i);
	return res;
}



class PlaneSweepStereoGPU : public PlaneSweepStereo
{
	Image depthmap;

	GLuint key_texture;
	vcl_vector<GLuint> ref_textures;

	// some of this buffers are double because
	// when updating one buffer we read from the other
	GLuint best_depth_texture[2];  // a color buffer storing the best depth (RED) and the best scores (GREEN)
	int current_best_depth_texture;

	GLuint depth_score_texture[2]; // a color buffer storing the sorted best scores found at current depth
	int current_depth_score_texture;

	GLuint agregated_score_texture;    // a color image storing the agregated score on its RED channel
	GLuint warped_image_texture;    // a color image storing the warped image and potentially its matching score
	GLuint tmp_texture;    // a temporal texture


public:
	PlaneSweepStereoGPU();
	void compute(int nplanes, CorrelationType ct, int kernel_size);
	Image get_depthmap();

private:


	void load_image_texture(GLuint texID, Image &im);
	void load_image_textures();
	void release_image_textures();
	void allocate_score_depth_textures();
	void release_score_depth_textures();

	void new_compute(int nplanes, CorrelationType ct, int kernel_size);

	void prepare_key_texture_ZNCC(int key_texture);
	void compute_grayscale_and_square(GLuint dest_texture, GLuint I_texture);

	void compute_warped_image(int r, double depth, CorrelationType ct);

	void agregate_score(CorrelationType ct, int kernel_size);
	void agregate_score_SD();
	void agregate_score_SSD(int kernel_size);
	void agregate_score_ZNCC(int kernel_size);

	void update_depth_score(int r, float depth);
	void update_best_depth(float depth);
	void read_best_depth_texture();

	vnl_float_3x3 image_to_image_homography_in_texture_coordinates(int r, float depth);

	void clear_texture(GLuint texID,float r=0, float g=0, float b=0, float a=0);
	void save_texture(GLuint texID, const char *filename, ... );
	void convolve_texture(GLuint dest, GLuint source, float weights[], float offsets_x[], float offsets_y[], int kernel_size);
	void convolve_textureRGBA(GLuint dest, GLuint source, float weightsRGBA[][4], float offsets_x[], float offsets_y[], int kernel_size);
	void gaussian_convolution_kernel(float *weights,float *offsetsx, float *offsetsy, int kernel_size);

	void draw_bounding_box();
	void draw_canonical_rectangle();
	void place_opengl_camera();
};

#endif //__PlaneSweepStereoGPU_h__