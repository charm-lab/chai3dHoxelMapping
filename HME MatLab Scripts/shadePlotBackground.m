% Shade only the backgrounds of training data

function [] = shadePlotBackground(lastTrainVal, firstTestVal)

% Pull in Parameter from workspace:
alphaVal = evalin('base', 'alphaVal');
yVal = evalin('base', 'yVal');
% xT = evalin('base', 'xT'); % X-vlaue of test-train border
% minY =  evalin('base', 'minY');
% maxY =  evalin('base', 'maxY');

minY = min(ylim); 
maxY = max(ylim);
minX = min(xlim);
xT = 0.5*(lastTrainVal+firstTestVal); % X-vlaue of test-train border
   
v = [minX minY; xT minY; xT maxY; minX maxY];
f = [1 2 3 4];
shadeColor = [0.5 0.5 0.5];

patch('Faces',f,'Vertices',v,'FaceColor',num2str(shadeColor),...
    'EdgeColor',num2str(shadeColor),...
    'EdgeAlpha',alphaVal,'FaceAlpha',alphaVal); hold on;

% Section labels:
textYCoord = maxY-yVal;
text(3.5, textYCoord,...
    "Training", "HorizontalAlignment","center", "FontSize", 20)
text(8, textYCoord,...
    "Testing", "HorizontalAlignment","center", "FontSize", 20)
% text(0.5*(xAxis(3)+xAxis(4)), textYCoord,...
%     "Training", "HorizontalAlignment","center", "FontSize", 20)
% text(xAxis(8), textYCoord,...
%     "Testing", "HorizontalAlignment","center", "FontSize", 20)
% Reset axes so patch doesn't move them
xlim([minX max(xlim)]);
ylim([minY maxY]);

end