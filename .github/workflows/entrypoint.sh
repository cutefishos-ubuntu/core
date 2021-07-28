#!/usr/bin/env bash

set -x

UPSTREAM_REPO=$1
UPSTREAM_BRANCH=$2
LOCAL_BRANCH=$3
GITHUB_TOKEN=$4

if [[ -z "$UPSTREAM_REPO" ]]; then
  echo "Missing \$UPSTREAM_REPO"
  exit 1
fi

if [[ -z "$UPSTREAM_BRANCH" ]]; then
  echo "Missing \$UPSTREAM_BRANCH"
  exit 1
fi

if [[ -z "$LOCAL_BRANCH" ]]; then
  echo "Missing \$LOCAL_BRANCH"
  exit 1
fi


if ! echo "$UPSTREAM_REPO" | grep '\.git'; then
  UPSTREAM_REPO="https://github.com/${UPSTREAM_REPO_PATH}.git"
fi

echo "UPSTREAM_REPO=$UPSTREAM_REPO"
echo "UPSTREAM_BRANCH=$UPSTREAM_BRANCH"
echo "LOCAL_BRANCH=$LOCAL_BRANCH"

git clone "https://github.com/${GITHUB_REPOSITORY}.git" work
cd work || { echo "Missing work dir" && exist 2 ; }

git config user.name "${GITHUB_ACTOR}"
git config user.email "${GITHUB_ACTOR}@users.noreply.github.com"
#git remote set-url origin "https://x-access-token:${GITHUB_TOKEN}@github.com/${GITHUB_REPOSITORY}.git"

git remote set-url origin "https://$GITHUB_ACTOR:$GITHUB_TOKEN@github.com/$GITHUB_REPOSITORY"
git remote add upstream "$UPSTREAM_REPO"
git fetch upstream #$UPSTREAM_BRANCH:$UPSTREAM_BRANCH $UPSTREAM_BRANCH
git remote -v

git checkout $LOCAL_BRANCH

MERGE_RESULT=$(git merge upstream/$UPSTREAM_BRANCH)
if [[ $MERGE_RESULT != *"Already up to date."* ]]; then
  git commit -m "Merged upstream"  
  git push origin $LOCAL_BRANCH
fi

cd ..
rm -rf work
