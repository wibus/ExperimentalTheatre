if __name__ == "__main__":
    import os
    
    outfilename = 'gl3w.h.out'
    infilename = 'gl3w.h'
    alreadyconfigured = False
    
    with open(outfilename, 'w') as out:
        
        for line in open(infilename, 'r'):
            if 'GL3_EXPORT' in line:
                alreadyconfigured = True
                break
            line = line.replace(
                """#include \"glcorearb.h\"""",
                """#include \"glcorearb.h\"

                   #ifdef _WINDOWS
                   #   if defined(GL3_LIBRARY)
                   #       define GL3_EXPORT __declspec(dllexport)
                   #   else
                   #       define GL3_EXPORT __declspec(dllimport)
                   #   endif
                   #else
                   #   define GL3_EXPORT
                   #endif""")
            line = line.replace(
                'extern PFN',
                'extern GL3_EXPORT PFN')
            out.write(line)

    if alreadyconfigured:
        #os.remove(outfilename)
        print('File already configured for Windows dll export')
    else:
        os.remove(infilename)
        os.rename(outfilename,infilename)
        print('File now configured for Windows dll export')
        
        
