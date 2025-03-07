# Add set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_third_party_flatbuffers AND CONFIG_USE_middleware_eiq_tensorflow_lite_micro_third_party_gemmlowp AND CONFIG_USE_middleware_eiq_tensorflow_lite_micro_third_party_ruy)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/core/api/error_reporter.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/core/api/flatbuffer_conversions.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/core/api/op_resolver.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/core/api/tensor_utils.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/core/c/common.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/kernels/kernel_util.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/kernels/internal/portable_tensor_utils.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/kernels/internal/quantization_util.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/kernels/internal/tensor_utils.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/kernels/internal/reference/portable_tensor_utils.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/all_ops_resolver.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/debug_log.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/fake_micro_context.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/flatbuffer_utils.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/memory_helpers.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/micro_allocation_info.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/micro_allocator.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/micro_context.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/micro_graph.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/micro_interpreter.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/micro_log.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/micro_op_resolver.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/micro_profiler.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/micro_resource_variable.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/micro_string.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/micro_time.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/micro_utils.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/recording_micro_allocator.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/activations_common.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/add_common.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/add_n.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/arg_min_max.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/assign_variable.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/batch_to_space_nd.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/broadcast_args.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/broadcast_to.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/call_once.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/cast.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/ceil.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/circular_buffer.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/circular_buffer_common.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/comparisons.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/concatenation.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/conv_common.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/cumsum.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/depth_to_space.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/depthwise_conv_common.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/dequantize.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/dequantize_common.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/detection_postprocess.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/div.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/elementwise.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/elu.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/exp.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/expand_dims.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/fill.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/floor_div.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/floor_mod.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/fully_connected_common.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/gather.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/gather_nd.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/hard_swish.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/hard_swish_common.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/if.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/kernel_util.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/l2_pool_2d.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/l2norm.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/leaky_relu_common.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/log_softmax.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/logical.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/logical_common.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/logistic_common.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/lstm_eval_common.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/maximum_minimum.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/micro_tensor_utils.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/mirror_pad.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/mul_common.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/neg.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/pack.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/pooling_common.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/prelu.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/prelu_common.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/quantize_common.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/read_variable.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/reduce_common.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/resize_bilinear.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/resize_nearest_neighbor.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/round.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/select.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/shape.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/slice.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/softmax_common.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/space_to_batch_nd.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/space_to_depth.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/split.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/split_v.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/squared_difference.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/squeeze.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/sub_common.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/svdf_common.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/tanh.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/transpose.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/unpack.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/var_handle.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/while.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/kernels/zeros_like.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/arena_allocator/non_persistent_arena_buffer_allocator.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/arena_allocator/persistent_arena_buffer_allocator.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/arena_allocator/recording_single_arena_buffer_allocator.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/arena_allocator/single_arena_buffer_allocator.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/memory_planner/greedy_memory_planner.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/memory_planner/linear_memory_planner.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/memory_planner/non_persistent_buffer_planner_shim.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/tflite_bridge/flatbuffer_conversions_bridge.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/tflite_bridge/micro_error_reporter.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/schema/schema_utils.cpp
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DTF_LITE_STATIC_MEMORY
  )

  if(CONFIG_TOOLCHAIN STREQUAL iar)
    target_compile_options(${MCUX_SDK_PROJECT_NAME} PUBLIC
      --compiler_language=auto
      --dlib_config full
      --enable_restrict
      --fno-rtti
      --fno-exceptions
      --diag_suppress Go003,Pa050,Pa082,Pa084,Pa093,Pe069,Pe111,Pe161,Pe174,Pe177,Pe186,Pe188,Pe550,Pe611,Pe997,Pe1444
    )
  endif()
  if(CONFIG_TOOLCHAIN STREQUAL mdk)
    target_compile_options(${MCUX_SDK_PROJECT_NAME} PUBLIC
      -ffp-mode=full
      -fno-exceptions
      -std=gnu++11
      -Wno-c++17-extensions
    )
  endif()
  if(CONFIG_TOOLCHAIN STREQUAL armgcc)
    target_compile_options(${MCUX_SDK_PROJECT_NAME} PUBLIC
      -Wall
      -Wno-strict-aliasing
      -fno-rtti
      -fno-exceptions
      -Wno-sign-compare
      -Wno-deprecated-declarations
    )
  endif()
  if(CONFIG_TOOLCHAIN STREQUAL mcux)
    target_compile_options(${MCUX_SDK_PROJECT_NAME} PUBLIC
      -Wno-strict-aliasing
      -fno-rtti
      -fno-exceptions
      -Wno-sign-compare
      -Wno-deprecated-declarations
    )
  endif()

endif()

else()

message(SEND_ERROR "middleware_eiq_tensorflow_lite_micro dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
