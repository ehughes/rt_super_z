#
# Copyright (c) 2020, NXP
#
# SPDX-License-Identifier: Apache-2.0
#

board_runner_args(jlink "--device=MIMXRT685SFAWBR" "--reset-after-load")

include(${ZEPHYR_BASE}/boards/common/jlink.board.cmake)
