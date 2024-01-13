%   Creates an errorbar plot with trial data
%   Also assumes subjects are sorted columnwise
%   in the SubjectData cell
% Author: Jasmin E. Palmer

function [h] = createNSForceErrorBarPlot(meanVals, barVals,...
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
alphaVal = evalin('base', 'alphaVal');
yVal = evalin('base', 'yVal');
dataSeparation = 0.3;
minX = 0.5;

% Get the x-axis values including a buffer on the first and last indices:
% Set to even itnerval for now
xTickVals = [0:numMappings];

xAxis = xTickVals(2:end);

% % Subjects Plots for testing trial data only:
% p = 2;
% h = errorbar([xAxis(1) xAxis(2) xAxis(3)]+jitterVal,...
%     meanVals{1,p}, barVals{1,p}, plotMarker, ...
%     "MarkerFaceColor", testingMap5Color, ...
%     "Color", testingMap5Color, "MarkerSize", markerSize, "LineWidth", 2);
% hold on;
% 
% ylim([minY maxY])

p = 2;

% Create dummy points in black color to be cut off intentionally so legend
% icons appear in correct color
h = errorbar(0, 0, 0, 0, plotMarker, "MarkerFaceColor","k", ...
    "Color", "k", "MarkerSize", markerSize, "LineWidth", 2);

hold on;
% Plot the actual data:
%h = 
errorbar(xAxis, meanVals{1,p}, barVals{1,p}, plotMarker, ...
    "MarkerFaceColor", testingMap1Color, ...
    "Color", testingMap5Color, "MarkerSize", markerSize, "LineWidth", 2);

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


