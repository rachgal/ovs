#!/bin/sh

# Define text views (used by misc. bash scripts).
TEXT_VIEW_NORMAL_BLUE="\e[01;34m"
TEXT_VIEW_NORMAL_RED="\e[31m"
TEXT_VIEW_NORMAL_GREEN="\e[32m"
TEXT_VIEW_NORMAL_MAGENTA="\e[35m"
TEXT_VIEW_NORMAL_YELLOW="\e[93m"

TEXT_VIEW_NORMAL_PURPLE="\e[177m"
TEXT_VIEW_NORMAL_ORANGE="\e[209m"
TEXT_VIEW_NORMAL='\e[00m'

g_config_item_number=1
g_config_section_number=1

#==================================================================================================================
#
#==================================================================================================================
show_menu_option()
{
  local command_name=$1
  local command_description=$2
  echo -e "${TEXT_VIEW_NORMAL_BLUE}$command_name${TEXT_VIEW_NORMAL} $command_description"
}

#==================================================================================================================
#
#==================================================================================================================
show_config_section()
{
  local config_item=$1
  
  echo -e "${TEXT_VIEW_NORMAL_GREEN}$g_config_section_number. $config_item${TEXT_VIEW_NORMAL}"

  # Increment item index
  ((g_config_section_number++))
}

#==================================================================================================================
#
#==================================================================================================================
show_config_item()
{
  local config_item=$1
  
  echo -e "${TEXT_VIEW_NORMAL_BLUE}  $g_config_item_number. $config_item${TEXT_VIEW_NORMAL}"

  # Increment item index
  ((g_config_item_number++))
}

#==================================================================================================================
#
#==================================================================================================================
note_init()
{
  local config_item=$1
  
  echo -e "${TEXT_VIEW_NORMAL_RED}  Note: $config_item${TEXT_VIEW_NORMAL}"
}

#==================================================================================================================
#
#==================================================================================================================
note_add()
{
  local config_item=$1
  
  echo -e "${TEXT_VIEW_NORMAL_RED}        $config_item${TEXT_VIEW_NORMAL}"
}

