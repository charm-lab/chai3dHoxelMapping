function animateKinematicData()
boxPosX = evalin('base','boxPosX');
boxPosY = evalin('base','boxPosY');
boxPosZ = evalin('base','boxPosZ');

indexPosX = evalin('base','indexPosX');
indexPosY = evalin('base','indexPosY');
indexPosZ = evalin('base','indexPosZ');

thumbPosX = evalin('base','thumbPosX');
thumbPosY = evalin('base','thumbPosY');
thumbPosZ = evalin('base','thumbPosZ');

hoopPos = evalin('base','hoopPos');
targetPos = evalin('base','targetPos');

figure(5);
% hoopPos = -hoopPos; targetPos = -targetPos;
textXOffset = 0.01;
textYOffset = 0.04;
textZOffset = 0.05;

%Hoop:
plot3(hoopPos(1), hoopPos(2), hoopPos(3),"o", 'Color', [0,0,1],...
    "LineWidth", 2, "MarkerSize", 30);hold on;
%Hoop text:
text(hoopPos(1), hoopPos(2), hoopPos(3) - textZOffset,...
    "Hoop", "Color", "red", "fontSize", 16);
plot3(targetPos(1), targetPos(2), targetPos(3) ,"bo",...
    "LineWidth", 2, "MarkerSize", 30);
%Target:
plot3(targetPos(1), targetPos(2), targetPos(3) ,"bo",...
    "LineWidth", 2, "MarkerSize", 200);
%Target text:
text(targetPos(1), targetPos(2), targetPos(3) + textZOffset,...
    "Target", "Color", "red", "fontSize", 16);
%Start point***
plot3(0.15, -0.2, -0.02, "bo", "LineWidth", 2, "MarkerSize", 30);
%Start point text:
text(0.15 - textXOffset, -0.2 - textYOffset, -0.02 - textZOffset,...
    "Start" + newline +"Point", "Color", "red", "fontSize", 16);

set(gca, 'ZDir','reverse');

% Axis labels
xlabel('X'); ylabel('Y'); zlabel('Z');

%Set camera azimuth and elevation angles
view([-71,33]);
improvePlot;

%Animation:
for i=1:length(boxPosX)
    plot3(boxPosX(i), boxPosY(i), boxPosZ(i), '-b', "MarkerSize", 5, "LineWidth", 8);
    plot3(indexPosX(i), indexPosY(i), indexPosZ(i),'k-',"LineWidth", 4);
    plot3(thumbPosX(i), thumbPosY(i), thumbPosZ(i),'Color', [0.5 0.5 0.5],"LineWidth", 4);
    % Axis limits
    xlim([-0.25, 1]);
    ylim([-0.9, 1.25]);
    zlim([-1.25, 1.25]);
    legend("Cube", "Index Finger", "Thumb", "Location", "northeast");
    drawnow;
%     disp("draw")
    pause(0.001);
end

hold off;

% %Create the video file
% video = VideoWriter('plotAnimated.avi', 'Uncompressed AVI');
% open(video);
% writeVideo(video, F);
% close(video);

end