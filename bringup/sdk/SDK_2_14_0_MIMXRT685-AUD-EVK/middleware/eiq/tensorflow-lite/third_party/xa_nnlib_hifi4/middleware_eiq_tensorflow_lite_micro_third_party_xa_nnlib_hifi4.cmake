# Add set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_third_party_xa_nnlib_hifi4 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/algo/common/src/xa_nnlib_common_api.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/activations/hifi4/xa_nn_activations_16_16.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/activations/hifi4/xa_nn_activations_32_16.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/activations/hifi4/xa_nn_activations_32_32.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/activations/hifi4/xa_nn_activations_32_8.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/activations/hifi4/xa_nn_activations_8_8.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/activations/hifi4/xa_nn_activations_asym16_asym16.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/activations/hifi4/xa_nn_activations_asym8_asym8.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/activations/hifi4/xa_nn_activations_f32_f32.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/activations/hifi4/xa_nn_softmax_asym8_asym8.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/basic/hifi4/xa_nn_broadcast_8_8.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/basic/hifi4/xa_nn_dot_prod_16x16.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/basic/hifi4/xa_nn_elm_abs_f32.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/basic/hifi4/xa_nn_elm_add_f32.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/basic/hifi4/xa_nn_elm_add_quant16.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/basic/hifi4/xa_nn_elm_add_quant8.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/basic/hifi4/xa_nn_elm_ceil_f32.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/basic/hifi4/xa_nn_elm_compare_quant8.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/basic/hifi4/xa_nn_elm_cosine_f32.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/basic/hifi4/xa_nn_elm_div_f32.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/basic/hifi4/xa_nn_elm_logical_bool.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/basic/hifi4/xa_nn_elm_logn_f32.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/basic/hifi4/xa_nn_elm_minmax_8.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/basic/hifi4/xa_nn_elm_mul_16x16.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/basic/hifi4/xa_nn_elm_mul_acc_f32.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/basic/hifi4/xa_nn_elm_mul_f32.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/basic/hifi4/xa_nn_elm_mul_quant8.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/basic/hifi4/xa_nn_elm_neg_f32.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/basic/hifi4/xa_nn_elm_quantize.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/basic/hifi4/xa_nn_elm_round_f32.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/basic/hifi4/xa_nn_elm_rsqrt_f32.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/basic/hifi4/xa_nn_elm_sine_f32.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/basic/hifi4/xa_nn_elm_sqrt_f32.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/basic/hifi4/xa_nn_elm_square_f32.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/basic/hifi4/xa_nn_elm_squared_diff_quant8.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/basic/hifi4/xa_nn_elm_sub_f32.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/basic/hifi4/xa_nn_elm_sub_quant16.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/basic/hifi4/xa_nn_elm_sub_quant8.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/basic/hifi4/xa_nn_floor_f32.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/basic/hifi4/xa_nn_memmove.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/basic/hifi4/xa_nn_memmove_16.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/basic/hifi4/xa_nn_memset_f32.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/basic/hifi4/xa_nn_reduce_asym8s_asym8s.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/basic/hifi4/xa_nn_vec_interpolation_q15.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_circ_buf.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_conv1d_std_16x16.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_conv1d_std_8x16.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_conv1d_std_8x8.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_conv1d_std_asym8xasym8.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_conv1d_std_circ_buf.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_conv1d_std_f32.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_conv2d_depthwise.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_conv2d_depthwise_16x16.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_conv2d_depthwise_8x16.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_conv2d_depthwise_8x8.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_conv2d_depthwise_asym8xasym8.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_conv2d_depthwise_f32.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_conv2d_depthwise_sym8sxasym8s.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_conv2d_depthwise_sym8sxsym16s.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_conv2d_pointwise_16x16.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_conv2d_pointwise_8x16.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_conv2d_pointwise_8x8.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_conv2d_pointwise_asym8xasym8.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_conv2d_pointwise_f32.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_conv2d_pointwise_sym8sxasym8s.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_conv2d_pointwise_sym8sxsym16s.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_conv2d_std_16x16.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_conv2d_std_8x16.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_conv2d_std_8x8.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_conv2d_std_asym8xasym8.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_conv2d_std_circ_buf.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_conv2d_std_f32.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_conv2d_std_sym8sxasym8s.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_conv2d_std_sym8sxsym16s.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_matXvec_16x16_16_circ.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_matXvec_16x16_16_circ_nb.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_matXvec_8x16_16_circ.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_matXvec_8x16_16_circ_nb.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_matXvec_8x8_8_circ.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_matXvec_8x8_8_circ_nb.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_matXvec_asym8xasym8_asym8_circ.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_matXvec_asym8xasym8_asym8_circ_nb.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_matXvec_f32_circ.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_matXvec_f32_circ_nb.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_matXvec_sym8sxasym8s_asym8s_circ.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_matXvec_sym8sxsym16s_sym16s_circ.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4/xa_nn_transpose_conv_sym8sxsym16s.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/fc/hifi4/xa_nn_fully_connected.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/matXvec/hifi4/xa_nn_matXvec_16x16.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/matXvec/hifi4/xa_nn_matXvec_16x16_batch.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/matXvec/hifi4/xa_nn_matXvec_16x8.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/matXvec/hifi4/xa_nn_matXvec_8x16.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/matXvec/hifi4/xa_nn_matXvec_8x16_batch.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/matXvec/hifi4/xa_nn_matXvec_8x8.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/matXvec/hifi4/xa_nn_matXvec_8x8_batch.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/matXvec/hifi4/xa_nn_matXvec_asym8sxasym8s.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/matXvec/hifi4/xa_nn_matXvec_asym8xasym8.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/matXvec/hifi4/xa_nn_matXvec_asym8xasym8_batch.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/matXvec/hifi4/xa_nn_matXvec_f32.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/matXvec/hifi4/xa_nn_matXvec_f32_batch.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/matXvec/hifi4/xa_nn_matXvec_sym8sxasym8s.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/matXvec/hifi4/xa_nn_matXvec_sym8sxsym16s.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/matXvec/hifi4/xa_nn_matmul_16x16.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/matXvec/hifi4/xa_nn_matmul_8x16.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/matXvec/hifi4/xa_nn_matmul_8x8.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/matXvec/hifi4/xa_nn_matmul_asym8sxasym8s.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/matXvec/hifi4/xa_nn_matmul_asym8xasym8.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/matXvec/hifi4/xa_nn_matmul_f32.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/matXvec/hifi4/xa_nn_matmul_sym8sxasym8s.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/matXvec/hifi4/xa_nn_matmul_sym8sxsym16s.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/norm/hifi4/xa_nn_l2_norm_asym8s.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/norm/hifi4/xa_nn_l2_norm_f32.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/pool/hifi4/xa_nn_avgpool.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/pool/hifi4/xa_nn_avgpool_16.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/pool/hifi4/xa_nn_avgpool_16_nhwc.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/pool/hifi4/xa_nn_avgpool_8.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/pool/hifi4/xa_nn_avgpool_8_nhwc.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/pool/hifi4/xa_nn_avgpool_asym8.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/pool/hifi4/xa_nn_avgpool_asym8_nhwc.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/pool/hifi4/xa_nn_avgpool_f32.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/pool/hifi4/xa_nn_avgpool_f32_nhwc.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/pool/hifi4/xa_nn_inv_256_tbl.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/pool/hifi4/xa_nn_maxpool.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/pool/hifi4/xa_nn_maxpool_16.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/pool/hifi4/xa_nn_maxpool_16_nhwc.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/pool/hifi4/xa_nn_maxpool_8.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/pool/hifi4/xa_nn_maxpool_8_nhwc.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/pool/hifi4/xa_nn_maxpool_asym8.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/pool/hifi4/xa_nn_maxpool_asym8_nhwc.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/pool/hifi4/xa_nn_maxpool_f32.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/pool/hifi4/xa_nn_maxpool_f32_nhwc.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/reorg/hifi4/xa_nn_batch_to_space_nd_8.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/reorg/hifi4/xa_nn_depth_to_space_8.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/reorg/hifi4/xa_nn_pad_16.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/reorg/hifi4/xa_nn_pad_8.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/reorg/hifi4/xa_nn_space_to_batch_nd_8.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/reorg/hifi4/xa_nn_space_to_depth_8.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/reorg/hifi4/xa_nn_stride_slice_int16.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/reorg/hifi4/xa_nn_stride_slice_int8.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/layers/cnn/src/xa_nn_cnn_api.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/layers/gru/src/xa_nn_gru_api.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/layers/lstm/src/xa_nn_lstm_api.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/ndsp/hifi4/src/expf_tbl.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/ndsp/hifi4/src/inff_tbl.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/ndsp/hifi4/src/inv2pif_tbl.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/ndsp/hifi4/src/lognf_tbl.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/ndsp/hifi4/src/nanf_tbl.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/ndsp/hifi4/src/pow2f_tbl.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/ndsp/hifi4/src/scl_sigmoidf_hifi4.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/ndsp/hifi4/src/scl_tanhf_hifi4.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/ndsp/hifi4/src/sinf_tbl.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/ndsp/hifi4/src/sqrt2f_tbl.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/ndsp/hifi4/src/tanhf_tbl.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/ndsp/hifi4/src/vec_alognf_hifi4.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/ndsp/hifi4/src/vec_cosinef_hifi4.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/ndsp/hifi4/src/vec_lognf_hifi4.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/ndsp/hifi4/src/vec_relu32x32_hifi4.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/ndsp/hifi4/src/vec_reluf_hifi4.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/ndsp/hifi4/src/vec_sigmoid32x32_hifi4.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/ndsp/hifi4/src/vec_sigmoidf_hifi4.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/ndsp/hifi4/src/vec_sinef_hifi4.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/ndsp/hifi4/src/vec_softmax32x32_hifi4.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/ndsp/hifi4/src/vec_softmaxf_hifi4.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/ndsp/hifi4/src/vec_tanh32x32_hifi4.c
  ${CMAKE_CURRENT_LIST_DIR}/algo/ndsp/hifi4/src/vec_tanhf_hifi4.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
  ${CMAKE_CURRENT_LIST_DIR}/algo/common/include
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/basic/hifi4
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/cnn/hifi4
  ${CMAKE_CURRENT_LIST_DIR}/algo/kernels/pool/hifi4
  ${CMAKE_CURRENT_LIST_DIR}/algo/ndsp/hifi4/include
  ${CMAKE_CURRENT_LIST_DIR}/include
  ${CMAKE_CURRENT_LIST_DIR}/include/nnlib
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DNNLIB_V2
    -DMODEL_INT16
    -Dhifi4
  )

endif()

