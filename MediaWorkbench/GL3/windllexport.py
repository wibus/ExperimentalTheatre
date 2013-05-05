if __name__ == "__main__":
    import os
    
    outfilename = 'gl3w.h.out'
    infilename = 'gl3w.h'
    alreadyconfigured = False
    
    with open(outfilename, 'w') as out:
        
        for line in open(infilename, 'r'):
            if line.find('CellarWorkbench'):
                alreadyconfigured = True
                break
            line = line.replace(
                '#include <GL3/gl3.h>',
                '#include <libCellarWorkbench_global.h>\n#include <GL3/gl3.h>')
            line = line.replace(
                'extern PFN',
                'extern CELLAR_EXPORT PFN')
            out.write(line)

    if alreadyconfigured:
        os.remove(outfilename)
        print('File already configured for Windows dll export')
    else:
        os.remove(infilename)
        os.rename(outfilename,infilename)
        print('File now configured for Windows dll export')
        
        
