project(images)

# Title Background
if(NOT MCPI_SERVER_MODE)
    install(
        FILES "vanilla.png"
        DESTINATION "${MCPI_INSTALL_DIR}/data/images/gui"
        RENAME "title_reborn.png"
    )
endif()

# 1.18 Title Background
if(NOT MCPI_SERVER_MODE)
    install(
        FILES "1.18.png"
        DESTINATION "${MCPI_INSTALL_DIR}/data/images/gui"
        RENAME "titleBG.png"
    )
endif()