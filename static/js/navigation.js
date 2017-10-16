/* jshint browser: true */

(function () {
    var navTop = document.getElementsByClassName('navigation top')[0],
        navFlip = navTop.getElementsByClassName('flip'),
        Animations = {},
        x = 0;

    Animations.Hover = {
        Flip: function() {
            if (this.style.webkitAnimationPlayState !== "running") {
                this.className = "flip hover";
            }
        }
    };

    Animations.Unhover = {
        Flip: function () {
            if (this.style.webkitAnimationPlayState !== "running") {
                this.className = "flip unhover";
            }
        }
    };

    for (x = 0; x < navFlip.length; x = x + 1) {
        navFlip[x].addEventListener('mouseover', Animations.Hover.Flip);
        navFlip[x].addEventListener('mouseout', Animations.Unhover.Flip);
    }
})();
