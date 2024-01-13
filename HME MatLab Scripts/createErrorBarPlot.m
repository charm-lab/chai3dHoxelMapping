%   Creates an errorbar plot with traiing trial data ONLY
%   Also assumes subjects are sorted columnwise
%   in the SubjectData cell
% Author: Jasmin E. Palmer

function [h] = createErrorBarPlot(meanVals, barVals,...
    plotTitle, xAxisLabel, yAxisLabel)

% Pull variables from workspace
testingMap1Color = evalin('base','testingMap1Color');
testingMap3Color = evalin('base','testingMap3Color');
testingMap5Color = evalin('base','testingMap5Color');
jitterVal = evalin('base','jitterVal');
numMappings = evalin('base','numMappings');
plotMarker = evalin('base','plotMarker');
markerSize = evalin('base', 'markerSize');
numExperimentTypes = evalin('base', 'numExperimentTypes');
minY =  evalin('base', 'minY');
maxY =  evalin('base', 'maxY');
% Plots for testing data only:

xAxis = (1:numMappings)-jitterVal;

p = 2;
% Plot the actual data:
h = errorbar(xAxis, meanVals{1,p}, barVals{1,p}, plotMarker, ...
    "MarkerFaceColor", testingMap1Color, ...
    "Color", testingMap5Color, "MarkerSize", markerSize, "LineWidth", 2);
hold on;
ylim([minY maxY])
  
% Make this a Multi-Color plot
colors = [testingMap1Color testingMap3Color testingMap5Color];
for i = 1:numMappings
 errorbar(xAxis(i), meanVals{1,p}(i), barVals{1,p}(i), plotMarker, ...
        "MarkerFaceColor", colors(i), ...
        "Color", colors(i), "MarkerSize", markerSize, "LineWidth", 2);
hold on;
    ylim([minY maxY])
end

% Plot Details
xlim([0.5 numMappings+0.5]);
xticks(1:numMappings);
tickLabels = ["Dual Tactor", "Single Tactor", "Control"];
set(gca,'xTick', [1:numMappings],'xticklabel', tickLabels); %#ok<NBRAK>
xlabel(xAxisLabel); ylabel(yAxisLabel);
% title(plotTitle);

hold off;
end


