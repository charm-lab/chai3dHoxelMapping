%   Creates an combined and color-coded errorbar plot with the visible and 
%   invible cube group data
%   Also assumes subjects are sorted columnwise:
%   [visibleCube | invisibleCube]
%   in the SubjectData cell

function [combinedErrorBarPlot] = ...
    createCombinedErrorBarPlot(plotType, plotTitle, xAxisLabel, yAxisLabel)

%Pull variables from workspace
visCubeColor = evalin('base','visCubeColor');
invisCubeColor = evalin('base','invisCubeColor');
indexVisColor = evalin('base','indexVisColor');
indexInvisColor = evalin('base','indexInvisColor');
thumbVisColor = evalin('base','thumbVisColor');
thumbInvisColor = evalin('base','thumbInvisColor');
boxVisColor = evalin('base','boxVisColor');
boxInvisColor = evalin('base','boxInvisColor');

jitterVal = evalin('base','jitterVal');
numMappings = evalin('base','numMappings');
visPlotMarker = evalin('base','visPlotMarker');
invisPlotMarker = evalin('base','invisPlotMarker');
markerSize = evalin('base', 'markerSize');

dataSeparation = 0.3;

%Pull parameters from workspace
if(plotType == "pathLengths")
    numCols = 3;
    visIndexPathLength = evalin('base','visIndexPathLength');
    invisIndexPathLength = evalin('base','invisIndexPathLength');
    visIndexPathLengthStdVals = evalin('base','visIndexPathLengthStdVals');
    invisIndexPathLengthStdVals = evalin('base','invisIndexPathLengthStdVals');
    
    visThumbPathLength = evalin('base','visThumbPathLength');
    invisThumbPathLength = evalin('base','invisThumbPathLength');
    visThumbPathLengthStdVals = evalin('base','visThumbPathLengthStdVals');
    invisThumbPathLengthStdVals = evalin('base','invisThumbPathLengthStdVals');
    
    visBoxPathLength = evalin('base','visBoxPathLength');
    invisBoxPathLength = evalin('base','invisBoxPathLength');
    visBoxPathLengthStdVals = evalin('base','visBoxPathLengthStdVals');
    invisBoxPathLengthStdVals = evalin('base','invisBoxPathLengthStdVals');
    
    %vis indexplot
    vec1 = (1:numMappings)-jitterVal;
    h1 = errorbar(vec1, ...
        visIndexPathLength, visIndexPathLengthStdVals, ...
        visPlotMarker, "MarkerFaceColor", str2num(indexVisColor), ...
        "Color", str2num(indexVisColor), "MarkerSize", markerSize, ...
        "LineWidth", 2);
    hold on;
    %invis indexplot
    vec2 = (1:numMappings)+jitterVal;
    h2 = errorbar(vec2, ...
        invisIndexPathLength, invisIndexPathLengthStdVals, ...
        invisPlotMarker, "Color", str2num(indexInvisColor),...
        "MarkerSize", markerSize, "LineWidth", 2);
    
    %vis thumb plot
    vec3 = 5+(1:numMappings)-jitterVal+dataSeparation;
    h3 = errorbar(vec3,...
        visThumbPathLength, visThumbPathLengthStdVals, ...
        visPlotMarker, "MarkerFaceColor", str2num(thumbVisColor), ...
        "Color", str2num(thumbVisColor), "MarkerSize", markerSize, ...
        "LineWidth", 2);
    %invis thumbplot
    vec4 = 5+(1:numMappings)+jitterVal+dataSeparation;
    h4 = errorbar(vec4,...
        invisThumbPathLength, invisThumbPathLengthStdVals, ...
        invisPlotMarker, "Color", str2num(thumbInvisColor), ...
        "MarkerSize", markerSize, "LineWidth", 2);
    
    %vis box plot
    vec5 = 10+(1:numMappings)-jitterVal+2*dataSeparation;
    h5 = errorbar(vec5,...
        visBoxPathLength, visBoxPathLengthStdVals, ...
        visPlotMarker, "MarkerFaceColor", str2num(boxVisColor), ...
        "Color", str2num(boxVisColor), "MarkerSize", markerSize, ...
        "LineWidth", 2);
    %invis boxplot
    vec6 = 10+(1:numMappings)+jitterVal+2*dataSeparation;
    h6 = errorbar(vec6,...
        invisBoxPathLength, invisBoxPathLengthStdVals, ...
        invisPlotMarker, "Color", str2num(boxInvisColor), ...
        "MarkerSize", markerSize, "LineWidth", 2);
    
    %Line to separate index and thumb plots
    xline((vec2(end)+ vec3(1))/2)
    
    %Line to separate thumb and box plots
    xline((vec4(end)+ vec5(1))/2)
    
    legend([h1(1), h2(1), h3(1), h4(1), h5(1), h6(1)],...
        "Index Visible", "Index Invisible", ...
        "Thumb Visible", "Thumb Invisible", ...
        "Cube Visible", "Cube Invisible", ...
        "Location", "northeast");
    
    xTickVals = [0, 1:5, (6:10)+dataSeparation, (11:15)+2*dataSeparation];
    
elseif (plotType == "normal-shear")
    numCols = 2;
    visIndexNormalMean = evalin('base','visIndexNormalMean');
    invisIndexNormalMean = evalin('base','invisIndexNormalMean');
    visIndexNormalMeanStdVals = evalin('base','visIndexNormalMeanStdVals');
    invisIndexNormalMeanStdVals = evalin('base','invisIndexNormalMeanStdVals');
    
    visIndexShearMean = evalin('base','visIndexShearMean');
    invisIndexShearMean = evalin('base','invisIndexShearMean');
    visIndexShearMeanStdVals = evalin('base','visIndexShearMeanStdVals');
    invisIndexShearMeanStdVals = evalin('base','invisIndexShearMeanStdVals');
    
    visThumbNormalMean = evalin('base','visThumbNormalMean');
    invisThumbNormalMean = evalin('base','invisThumbNormalMean');
    visThumbNormalMeanStdVals = evalin('base','visThumbNormalMeanStdVals');
    invisThumbNormalMeanStdVals = evalin('base','invisThumbNormalMeanStdVals');
    
    visThumbShearMean = evalin('base','visThumbShearMean');
    invisThumbShearMean = evalin('base','invisThumbShearMean');
    visThumbShearMeanStdVals = evalin('base','visThumbShearMeanStdVals');
    invisThumbShearMeanStdVals = evalin('base','invisThumbShearMeanStdVals');
    
    %vis index normal plot
    vec1 = (1:numMappings)-jitterVal;    
    vec2 = (1:numMappings)+jitterVal;
    h1 = errorbar(vec1, ...
        visIndexNormalMean, visIndexNormalMeanStdVals, ...
        "d", "MarkerFaceColor", str2num(indexVisColor), ...
        "Color", str2num(indexVisColor), "MarkerSize", markerSize, ...
        "LineWidth", 2);
    hold on;
    %invis index normal plot
    h2 = errorbar(vec2, ...
        invisIndexNormalMean, invisIndexNormalMeanStdVals, ...
        "d", "Color", str2num(indexInvisColor),...
        "MarkerSize", markerSize, "LineWidth", 2);
    
    %vis index shear plot
    h3 = errorbar(vec1,...
        visIndexShearMean, visIndexShearMeanStdVals, ...
        visPlotMarker, "MarkerFaceColor", str2num(indexVisColor), ...
        "Color", str2num(indexVisColor), "MarkerSize", markerSize, ...
        "LineWidth", 2);
    %invis index shear plot
    h4 = errorbar(vec2,...
        invisIndexShearMean, invisIndexShearMeanStdVals, ...
        invisPlotMarker, "Color", str2num(indexInvisColor), ...
        "MarkerSize", markerSize, "LineWidth", 2);
        
    vec3 = 5+(1:numMappings)-jitterVal+dataSeparation;    
    vec4 = 5+(1:numMappings)+jitterVal+dataSeparation;
    
    %vis thumb normal plot
    h5 = errorbar(vec3, ...
        visThumbNormalMean, visThumbNormalMeanStdVals, ...
        "d", "MarkerFaceColor", str2num(thumbVisColor), ...
        "Color", str2num(thumbVisColor), "MarkerSize", markerSize, ...
        "LineWidth", 2);
    hold on;
    %invis thumb normal plot
    h6 = errorbar(vec4, ...
        invisThumbNormalMean, invisThumbNormalMeanStdVals, ...
        "d", "Color", str2num(thumbInvisColor),...
        "MarkerSize", markerSize, "LineWidth", 2);
    
    %vis thumb shear plot
    h7 = errorbar(vec3,...
        visThumbShearMean, visThumbShearMeanStdVals, ...
        visPlotMarker, "MarkerFaceColor", str2num(thumbVisColor), ...
        "Color", str2num(thumbVisColor), "MarkerSize", markerSize, ...
        "LineWidth", 2);
    %invis thumb shear plot
    h8 = errorbar(vec4,...
        invisThumbShearMean, invisThumbShearMeanStdVals, ...
        invisPlotMarker, "Color", str2num(thumbInvisColor), ...
        "MarkerSize", markerSize, "LineWidth", 2);
    
    %Line to separate thumb and box plots
    xline((vec2(end)+ vec3(1))/2)
    
    legend([h3(1), h4(1),h1(1), h2(1),  h7(1), h8(1), h5(1), h6(1)],...
        "Index Shear Force Visible", ...
        "Index Shear Force Invisible",...
        "Index Normal Force Visible", ...
        "Index Normal Force Invisible",...
        "Thumb Shear Force Visible", ...
        "Thumb Shear Force Invisible",...
        "Thumb Normal Force Visible", ...
        "Thumb Normal Force Invisible",...
        "Location", "northwest");
    
    xTickVals = [0, 1:5, (6:10)+dataSeparation];
    
elseif (plotType == "boxDrops-categorized")
    numCols = 2;
    visGoodBoxDropMean = evalin('base','visGoodBoxDropMean');
    invisGoodBoxDropMean = evalin('base','invisGoodBoxDropMean');
    visGoodBoxDropStd= evalin('base','visGoodBoxDropStd');
    invisGoodBoxDropStd = evalin('base','invisGoodBoxDropStd');
    
    visBadBoxDropMean = evalin('base','visBadBoxDropMean');
    invisBadBoxDropMean = evalin('base','invisBadBoxDropMean');
    visBadBoxDropStd= evalin('base','visBadBoxDropStd');
    invisBadBoxDropStd = evalin('base','invisBadBoxDropStd');
    
    goodSubjectNum= evalin('base','goodSubjectNum');
    badSubjectNum = evalin('base','badSubjectNum');
    cutoff = evalin('base','cutoff');
    
    %vis good plot
    vec1 = (1:numMappings)-jitterVal;    
    vec2 = (1:numMappings)+jitterVal;
    h1 = errorbar(vec1, ...
        visGoodBoxDropMean, visGoodBoxDropStd, ...
        "s", "MarkerFaceColor", str2num(visCubeColor), ...
        "Color", str2num(visCubeColor), "MarkerSize", markerSize, ...
        "LineWidth", 2);
    hold on;
    %invis good plot
    h2 = errorbar(vec2, ...
        invisGoodBoxDropMean, invisGoodBoxDropStd, ...
        "s", "Color", str2num(invisCubeColor),...
        "MarkerSize", markerSize, "LineWidth", 2); 
    text(4.5,12, strcat("<", num2str(cutoff), " Drops"), "fontSize", 16);
    text(5,11, strcat("n=",num2str(length(goodSubjectNum))), "fontSize", 16);
    
    vec3 = 5+(1:numMappings)-jitterVal+dataSeparation;    
    vec4 = 5+(1:numMappings)+jitterVal+dataSeparation;
    
    %vis bad plot
    h3 = errorbar(vec3, ...
        visBadBoxDropMean, visBadBoxDropStd, ...
        "s", "MarkerFaceColor", str2num(visCubeColor), ...
        "Color", str2num(visCubeColor), "MarkerSize", markerSize, ...
        "LineWidth", 2);
    hold on;
    %invis bad plot
    h4 = errorbar(vec4, ...
        invisBadBoxDropMean, invisBadBoxDropStd, ...
        "s", "Color", str2num(invisCubeColor),...
        "MarkerSize", markerSize, "LineWidth", 2);        
    text(9.25,12, strcat("\geq", num2str(cutoff), " Drops"), "fontSize", 16);
    text(10,11, strcat("n=",num2str(length(badSubjectNum))), "fontSize", 16);
    %Line to separate thumb and box plots
    xline((vec2(end)+ vec3(1))/2)
    
    legend([h1(1), h2(1)],...
        "Visible Cube", ...
        "Invisible Cube",...
        "Location", "northwest");
    
    xTickVals = [0, 1:5, (6:10)+dataSeparation];
    
else
    disp("invalid plot type -- FAIL")
    return;
end


%Plot Details
n=numCols;
xlim([0 max(xTickVals)+2*dataSeparation]);
% tickLabels = [" ", repmat(["1", "2", "3", "4", "Control"], 1, n)];

tickLabels = [" ", repmat(["1", "Control"], 1, n)];
xticks(xTickVals);
set(gca, 'xTick', xTickVals, 'xticklabel', tickLabels); %#ok<NBRAK>
% xticks(0:(6*n-n));
% set(gca,'xTick', [0:(6*n-n)],'xticklabel', tickLabels); %#ok<NBRAK>
% set(gca,'xticklabel', tickLabels); %#ok<NBRAK>
xlabel(xAxisLabel); ylabel(yAxisLabel);
title(plotTitle);
hold off;

end


