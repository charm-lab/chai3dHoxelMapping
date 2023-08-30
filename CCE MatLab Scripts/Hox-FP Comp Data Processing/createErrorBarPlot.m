%   Creates an errorbar plot with trial data
%   Also assumes subjects are sorted columnwise
%   in the SubjectData cell

% function [visCubeErrorBarPlot, cubeMeanVals, cubeStdVals ] = ...
%     createErrorBarPlot(map1, map2, map3, map4, map5,...
%     plotTitle, xAxisLabel, yAxisLabel)

function [errorBarPlot] = createErrorBarPlot(meanVals, stdVals,...
    plotTitle, xAxisLabel, yAxisLabel)

% Pull variables from workspace
visCubeColor = evalin('base','visCubeColor');
jitterVal = evalin('base','jitterVal');
numMappings = evalin('base','numMappings');
plotMarker = evalin('base','plotMarker');
markerSize = evalin('base', 'markerSize'); 

%Vis Cube Subjects Plots
errorBarPlot = errorbar((1:numMappings)-jitterVal,...
    meanVals, stdVals, plotMarker, ...
    "MarkerFaceColor", str2num(visCubeColor), ...
    "Color", str2num(visCubeColor), "MarkerSize", markerSize, "LineWidth", 2);
hold on;

%Plot Details
% xlim([0 6]); tickLabels = ["1", "2", "3", "4", "Control"];
% xticks(0:6); set(gca,'xTick', [1:numMappings],'xticklabel', tickLabels); %#ok<NBRAK>
xlim([0 numExperimentTypes]);
xticks([1 numExperimentTypes]);
tickLabels = ["1", "Control"];
set(gca,'xTick', [1:numMappings],'xticklabel', tickLabels); %#ok<NBRAK>
xlabel(xAxisLabel); ylabel(yAxisLabel);
title(plotTitle);
hold off;

end


