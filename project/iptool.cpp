#include "../iptools/core.h"
#include <string.h>
#include <vector>

using namespace std;

#define MAXLEN 256

// ROI structure
struct ROI {
    int x, y, width, height;
    string function;
    vector<int> params;

    ROI(int x, int y, int w, int h, string func, vector<int> p)
        : x(x), y(y), width(w), height(h), function(std::move(func)), params(std::move(p)) {}
};

int main (int argc, char** argv) {
    image src, tgt;
    FILE *fp;
    char str[MAXLEN];
    char infile[MAXLEN];
    char outfile[MAXLEN];
    char *pch;
    vector<ROI> rois;
    
    if ((fp = fopen(argv[1],"r")) == NULL) {
        fprintf(stderr, "Can't open file: %s\n", argv[1]);
        exit(1);
    }

    while(fgets(str,MAXLEN,fp) != NULL) {
        if (strncmp(str,"#",1)==0) continue;
        
        pch = strtok(str, " ");
        strcpy(infile, pch);

        pch = strtok(NULL, " ");
        strcpy(outfile, pch);

        // Read the number of ROIs
        pch = strtok(NULL, " ");
        int numberOfROIs = atoi(pch);

        // If there are ROIs, parse their information
        if (numberOfROIs > 0) {
            for (int i = 0; i < numberOfROIs; ++i) {
                int x = atoi(strtok(NULL, " "));
                int y = atoi(strtok(NULL, " "));
                int width = atoi(strtok(NULL, " "));
                int height = atoi(strtok(NULL, " "));
                string function(strtok(NULL, " "));

                // Parse additional parameters for the function
                vector<int> params;
                while ((pch = strtok(NULL, " ")) != NULL) {
                    params.push_back(atoi(pch));
                }

                rois.emplace_back(x, y, width, height, function, params);
            }
        }

        // Process the image
        src.read(infile);
        tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());

        // Apply functions to each ROI or the whole image
        if (!rois.empty()) {
            for (const auto& roi : rois) {
                // Apply the function only to the ROI
                if (roi.function == "add") {
					utility::addGrey(src, tgt, roi.params[0], roi.x, roi.y, roi.width, roi.height);
				} else if (roi.function == "binarize") {
					utility::binarize(src, tgt, roi.params[0], roi.x, roi.y, roi.width, roi.height);
				}
                // ADD MORE FUNCTIONS HERE 
			}
        } else {
			// Apply the function to the whole image
			pch = strtok(NULL, " ");
			if (pch != NULL) {
				if (strncmp(pch,"add",3)==0) {
					pch = strtok(NULL, " ");
					if (pch != NULL) utility::addGrey(src, tgt, atoi(pch), 0, 0, src.getNumberOfColumns(), src.getNumberOfRows());
				} else if (strncmp(pch,"binarize",8)==0) {
					pch = strtok(NULL, " ");
					if (pch != NULL) utility::binarize(src, tgt, atoi(pch), 0, 0, src.getNumberOfColumns(), src.getNumberOfRows());
				} else if (strncmp(pch,"scale",5)==0) {
					pch = strtok(NULL, " ");
					if (pch != NULL) utility::scale(src, tgt, atof(pch)); // Assuming scale applies to the whole image by default
				} else {
					printf("No function: %s\n", pch);
					continue;
				}
			}
		}
        tgt.save(outfile);
    }
    fclose(fp);
    return 0;
}

