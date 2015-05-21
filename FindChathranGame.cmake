# - Try to find game data for use in Chathran
# Once done this will define
#
#  CHGAME_FOUND - flag to signal Chathran game data is found
#  CHGAME_INCLUDE_DIR - the include directory (should contain a 'lua' folder, among others)
#

FIND_PATH(CHGAME_INCLUDE_DIR NAMES lua/start.lua
  PATHS
  ${PROJECT_SOURCE_DIR}/../chathran-game
  ${PROJECT_SOURCE_DIR}/../game
  ${PROJECT_SOURCE_DIR}/../chgame
)
 
IF(CHGAME_INCLUDE_DIR)
   SET(CHGAME_FOUND TRUE)
ENDIF(CHGAME_INCLUDE_DIR)

