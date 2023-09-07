function [r, p, al3, X, Y] = arc3(p1, p2, p3)
% arc3: Compute the center, radii, angle and the reference frame of the arc passing through 3 points
%
%   Input:
%     p1, p2, p3: row 3-vectors representing 3d space points, 
%       the arc starts at p1 goes trough p2 and finishes at p3
%   Output:
%     r: calculated radii
%     p: raw 3-vector representing the center of the arc
%     al3: the final angle of the arc
%     X, Y: row unit 3-vectors representing the reference frame of the plane where the arc lies, 
%       X vector is pointing to the p1 so the starting angle is always 0
%
%   Examples: 
%     Plot random arc:
%       arc3(rand(1, 3), rand(1, 3), rand(1, 3));
%     Calculate without plotting:
%       [r, p, al, X, Y] = arc3(rand(1, 3), rand(1, 3), rand(1, 3));
%
% Reference: https://en.wikipedia.org/wiki/Circumscribed_circle

%% prepare
    a = p2 - p1;
    b = p3 - p1;
    n = cross(a, b);
%% radius
    r = norm(a)*norm(b)*norm(a-b)/2/norm(n);
%% center
    p = cross(norm(a)^2*b-norm(b)^2*a, n)/2/norm(n)^2 + p1;
%% reference frame
    X = (p1-p)/r;
    Y = cross(X, n/norm(n));
%% start-stop angle
    al2 = atan2(dot(cross((p2-p)/r, X), n/norm(n)), dot((p2-p)/r, X));
    al3 = atan2(dot(cross((p3-p)/r, X), n/norm(n)), dot((p3-p)/r, X));

    al2 = mod(al2, 2*pi);
    al3 = mod(al3, 2*pi);

    if al2 > al3
        al3 = -2*pi+al3;
    end
%% draw a plot if no output required
    if nargout == 0
%% helper plot function
        plot3v = @(v, varargin)(plot3(v(:, 1), v(:, 2), v(:, 3), varargin{:}));
%% plot arc
        al = linspace(0, al3, 200);
        c = (cos(al)'*X + sin(al)'*Y)*r + p;
        plot3v(c, 'r', 'linewidth', 2);
        hold on;
%% plot circle
%         al = linspace(0, 2*pi, 200);
%         c = (cos(al)'*X + sin(al)'*Y)*r + p;
%         plot3v(c, 'b--');    
%% plot points
%         plot3v(p1, 'ro', 'MarkerSize', 10, 'LineWidth', 2);
%         plot3v(p2, 'go', 'MarkerSize', 10, 'LineWidth', 2);
%         plot3v(p3, 'bo', 'MarkerSize', 10, 'LineWidth', 2);
%% plot radii
%         plot3v([p; p1], 'r--');
%         plot3v([p; p2], 'g--');
%         plot3v([p; p3], 'b--');
%% plot center
%         plot3v(p, 'kx', 'MarkerSize', 10, 'LineWidth', 2);
%% plot settings        
%         hold off;
%         axis equal;
%         grid on;
    end
end

