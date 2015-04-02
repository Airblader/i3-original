#!perl
# vim:ts=4:sw=4:expandtab
#
# Please read the following documents before working on tests:
# • http://build.i3wm.org/docs/testsuite.html
#   (or docs/testsuite)
#
# • http://build.i3wm.org/docs/lib-i3test.html
#   (alternatively: perldoc ./testcases/lib/i3test.pm)
#
# • http://build.i3wm.org/docs/ipc.html
#   (or docs/ipc)
#
# • http://onyxneon.com/books/modern_perl/modern_perl_a4.pdf
#   (unless you are already familiar with Perl)
#
# Test that assigning a workspace to a list of outputs works
# correctly.
# Ticket: #555
use i3test i3_autostart => 0;

sub get_focused_output {
    my $tree = i3(get_socket_path())->get_tree->recv;
    my ($focused_id) = @{$tree->{focus}};
    my ($output) = grep { $_->{id} == $focused_id } @{$tree->{nodes}};
    return $output->{name};
}

#####################################################################
# 1: If the first output is present, check that it is used.
#####################################################################

my $config = <<EOT;
# i3 config file (v4)
font -misc-fixed-medium-r-normal--13-120-75-75-C-70-iso10646-1

# decoy workspaces to not open 'special' upon startup
workspace 1 output fake-0
workspace 2 output fake-1

workspace special output fake-0 fake-1

fake-outputs 1024x768+0+0,1024x768+1024+0
EOT

my $pid = launch_with_config($config);

cmd 'focus output fake-1';
is(get_focused_output(), 'fake-1', 'second output is focused');

cmd 'workspace special';

is(get_focused_output(), 'fake-0', 'workspace opened on the first output');

exit_gracefully($pid);

#####################################################################
# 2: If the first output is not present, check that the first present
#    one in the list is used.
#####################################################################

my $config = <<EOT;
# i3 config file (v4)
font -misc-fixed-medium-r-normal--13-120-75-75-C-70-iso10646-1

# decoy workspaces to not open 'special' upon startup
workspace 1 output fake-0
workspace 2 output fake-1

workspace special output notme-0 fake-0

fake-outputs 1024x768+0+0,1024x768+1024+0
EOT

my $pid = launch_with_config($config);

cmd 'focus output fake-1';
is(get_focused_output(), 'fake-1', 'second output is focused');

cmd 'workspace special';

is(get_focused_output(), 'fake-0', 'workspace opened on the first output');

exit_gracefully($pid);

#####################################################################
# 3: If the workspace is already present, but an assigned output
#    prior to its current one needs a workspace, check that it is
#    moved.
#####################################################################

my $config = <<EOT;
# i3 config file (v4)
font -misc-fixed-medium-r-normal--13-120-75-75-C-70-iso10646-1

workspace special output fake-0 fake-1

fake-outputs 1024x768+0+0,1024x768+1024+0
EOT

my $pid = launch_with_config($config);

cmd 'workspace special';
cmd 'move workspace to output fake-1';
cmd 'focus output fake-0';
is(get_focused_output(), 'fake-0', 'workspace opened on the first output');
is(focused_ws(), 'special');

exit_gracefully($pid);

# todo what happens if last ws on output and move it away -> does it come back?
# 4: ws present, but later in the list -> don't move it
# 5: assign to fake-1 fake-0 upon start and check its on fake-1

#####################################################################

done_testing;
