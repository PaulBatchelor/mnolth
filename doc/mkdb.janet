# open and clear wiki db

(ww-open "a.db")
(ww-clear)

# unlinked pages


# linked pages

(ww-add-link "index" "index.org")
(ww-add-link "btprnt" "btprnt.org")
(ww-add-link "gestvm" "gestvm.org")
(ww-add-link "building" "building.org")
(ww-add-link "mnort" "mnort.org")
(ww-add-link "wikidex" "wikidex.org")
(ww-add-link "fennel_mnort" "fennel_mnort.org")
(ww-add-link "crate" "crate.org")

# sync and close

(ww-sync)
(ww-close)
