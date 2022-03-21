add_compile_options(-I${CMAKE_CURRENT_SOURCE_DIR})

add_executable(archiver
    main.cpp
    Output/BitWriter.cpp
    Output/BitWriter.h
    Input/BitReader.cpp
    Input/BitReader.h
        Archiver/Encoder/Encoder.cpp
        Archiver/Encoder/Encoder.h
        Archiver/Haffman/HaffmanTree.cpp
        Archiver/Haffman/HaffmanTree.h
        Archiver/Haffman/Heap.cpp
        Archiver/Haffman/Heap.h
        Archiver/Decoder/Decoder.cpp
        Archiver/Decoder/Decoder.h
)
